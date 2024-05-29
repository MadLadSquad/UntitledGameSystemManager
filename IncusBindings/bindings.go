package main

import (
	"C"
	incus "github.com/lxc/incus/client"
	"github.com/lxc/incus/shared/api"
	"io"
	"os"
	"strconv"
	"strings"
	_ "sync"
	_ "unsafe"
)

type internalPulseSocketAppendType uint

const (
	InternalPulseSocketAppendTypeNone        internalPulseSocketAppendType = 1 << 0
	InternalPulseSocketAppendTypeHome        internalPulseSocketAppendType = 1 << 1
	InternalPulseSocketAppendTypeHomeAndNone                               = InternalPulseSocketAppendTypeNone | InternalPulseSocketAppendTypeHome
)

var (
	c                     incus.InstanceServer
	errorG                string
	err                   error
	hardcodedPulseSockets = map[string]internalPulseSocketAppendType{
		"/run/user/" + strconv.Itoa(os.Getuid()) + "/pulse/native":     InternalPulseSocketAppendTypeNone,
		"/var/run/user/" + strconv.Itoa(os.Getuid()) + "/pulse/native": InternalPulseSocketAppendTypeNone,
		"/pulse-native":         InternalPulseSocketAppendTypeHomeAndNone,
		"/.config/pulse/native": InternalPulseSocketAppendTypeHome,
		"/.pulse/native":        InternalPulseSocketAppendTypeHome,
		"/run/pulse/native":     InternalPulseSocketAppendTypeNone,
	}
)

func handleWait(operation incus.Operation) C.char {
	err = operation.Wait()
	if err != nil {
		errorG = err.Error()
		return -1
	}
	return 0
}

func handleWaitRemote(operation incus.RemoteOperation) {
	err = operation.Wait()
	if err != nil {
		errorG = err.Error()
	}
}

func getContainerHandle(name *C.char) (api.Instance, int) {
	containers, err := IncusGetContainers()
	if err != nil {
		errorG = err.Error()
		// -1 Couldn't fetch containers
		return api.Instance{}, -1
	}

	str := C.GoString(name)

	for _, container := range containers {
		if container.Name == str {
			return container, 0
		}
	}

	// -2 Couldn't find container
	return api.Instance{}, -2
}

func updateContainerState(name *C.char, action string, force bool, runOnActive bool) C.char {
	str := C.GoString(name)
	container, e := getContainerHandle(name)
	if e < 0 {
		errorG = "Error getting container with the following name: " + str + "; Error code: " + strconv.Itoa(e)
		return -1
	}

	if runOnActive == container.IsActive() {
		op, err := c.UpdateInstanceState(
			str, api.InstanceStatePut{Action: action, Timeout: -1, Force: force}, "")
		if err != nil {
			errorG = err.Error()
			return -1
		}
		handleWait(op)
		return 0
	}
	return 1
}

//export IncusGetError
func IncusGetError() *C.char {
	return C.CString(errorG)
}

//export IncusCreateConnection
func IncusCreateConnection() C.char {
	c, err = incus.ConnectIncusUnix("", nil)
	if err != nil {
		errorG = err.Error()
		return -1
	}
	return 0
}

//export IncusDestroyConnection
func IncusDestroyConnection() {
	c.Disconnect()
}

func IncusGetContainers() ([]api.Instance, error) {
	return c.GetInstances(api.InstanceTypeContainer)
}

//export IncusStartContainer
func IncusStartContainer(name *C.char) C.char {
	return updateContainerState(name, "start", false, false)
}

//export IncusStopContainer
func IncusStopContainer(name *C.char) C.char {
	return updateContainerState(name, "stop", true, true)
}

//export IncusRestartContainer
func IncusRestartContainer(name *C.char) C.char {
	return updateContainerState(name, "restart", true, true)
}

func getPulseSocketLocation() string {
	Home := os.Getenv("HOME")
	XdgRuntimeDir := os.Getenv("XDG_RUNTIME_DIR")
	PulseServer := os.Getenv("PULSE_SERVER")

	if XdgRuntimeDir != "" {
		if _, err = os.Stat(XdgRuntimeDir + "/pulse/native"); err == nil {
			return XdgRuntimeDir + "/pulse/native"
		}
	}

	if PulseServer != "" {
		unixLen := len("unix:")

		if len(PulseServer) > unixLen {
			if _, err = os.Stat(PulseServer[unixLen:]); err == nil {
				return PulseServer[unixLen:]
			}
		}
	}

	for key, val := range hardcodedPulseSockets {
		if (val & InternalPulseSocketAppendTypeHome) != 0 {
			if _, err = os.Stat(Home + key); err == nil {
				return Home + key
			}
		}
		if (val & InternalPulseSocketAppendTypeNone) != 0 {
			if _, err = os.Stat(key); err == nil {
				return key
			}
		}
	}

	return ""
}

//export IncusNewContainer
func IncusNewContainer(name *C.char, alias *C.char) C.char {
	d, err := incus.ConnectSimpleStreams("https://images.linuxcontainers.org", nil)
	if err != nil {
		return -1
	}
	aliasS, _, err := d.GetImageAlias(C.GoString(alias))
	if err != nil {
		return -1
	}
	image, _, err := d.GetImage(aliasS.Target)
	if err != nil {
		return -1
	}

	op, err := c.CopyImage(d, *image, nil)
	if err != nil {
		return -1
	}
	handleWaitRemote(op)

	pulseDir := getPulseSocketLocation()

	if pulseDir == "" {
		errorG = `Couldn't find the PulseAudio socket location. Please set the "PULSE_SERVER" environment variable to a location like "/pulse-native" or "$HOME/pulse-native"`
		return -1
	}

	req := api.InstancesPost{
		Name: C.GoString(name),
		Type: api.InstanceTypeContainer,
		Source: api.InstanceSource{
			Type:        "image",
			Fingerprint: image.Fingerprint,
		},
	}
	req.Config = map[string]string{
		"security.privileged":         "true",
		"security.nesting":            "true",
		"environment.PROTON_NO_ESYNC": "1",
		"environment.PULSE_SERVER":    "unix:/pulse-native",
		"environment.DISPLAY":         ":0",
	}

	req.Devices = map[string]map[string]string{
		"mygpu": {
			"type": "gpu",
		},
		"PASocket1": {
			"bind":         "container",
			"connect":      "unix:" + pulseDir,
			"gid":          "1000",
			"listen":       "unix:/pulse-native",
			"mode":         "0777",
			"security.gid": "1000",
			"security.uid": "1000",
			"type":         "proxy",
			"uid":          "1000",
		},
		"X0": {
			"bind":         "container",
			"connect":      "unix:/tmp/.X11-unix/X0",
			"gid":          "1000",
			"listen":       "unix:/tmp/.X11-unix/X0",
			"mode":         "0777",
			"security.gid": "1000",
			"security.uid": "1000",
			"type":         "proxy",
			"uid":          "1000",
		},
	}

	nop, err := c.CreateInstance(req)
	if err != nil {
		errorG = err.Error()
		return -1
	}
	handleWait(nop)

	return IncusStartContainer(name)
}

//export IncusDeleteContainer
func IncusDeleteContainer(name *C.char) C.char {
	str := C.GoString(name)
	if IncusStopContainer(name) == -1 {
		return -1
	}

	op, err := c.DeleteInstance(str)
	if err != nil {
		errorG = err.Error()
		return -1
	}

	handleWait(op)

	return 0
}

//export IncusExec
func IncusExec(name *C.char, command *C.char, bWait C.char) C.char {
	str := C.GoString(name)
	container, e := getContainerHandle(name)
	if e < 0 {
		errorG = "Error getting container with the following name: " + str + "; Error code: " + strconv.Itoa(e)
		return -1
	}

	// If active stop container
	if !container.IsActive() {
		errorG = "Couldn't exec on container that is not active!"
		return -1
	}

	cmd := C.GoString(command)
	cmds := strings.Split(cmd, "{{b}}")

	op, err := c.ExecInstance(str, api.InstanceExecPost{
		Command:     cmds,
		WaitForWS:   true,
		Interactive: false,
		Environment: map[string]string{},
		Width:       800,
		Height:      600,
	}, &incus.InstanceExecArgs{
		Stdin:  os.Stdin,
		Stdout: os.Stdout,
		Stderr: os.Stderr,
	})
	if err != nil {
		errorG = err.Error()
		return -1
	}
	if bWait != 0 {
		return handleWait(op)
	}

	return 0
}

//export IncusPushFile
func IncusPushFile(name *C.char, path *C.char, file *C.char) C.char {
	str := C.GoString(name)
	fl := C.GoString(file)

	if err != nil {
		errorG = "Couldn't read file data. File: " + C.GoString(path)
		return -1
	}

	f, err := os.Open(fl)
	if err != nil {
		errorG = err.Error()
		return -1
	}

	reader := io.ReadSeeker(f)

	err = c.CreateInstanceFile(str, C.GoString(path), incus.InstanceFileArgs{
		Content:   reader,
		UID:       0,    // Root
		GID:       0,    // Root
		Mode:      0775, // -rw-r--r--
		Type:      "file",
		WriteMode: "overwrite",
	})
	if err != nil {
		errorG = err.Error()
		return -1
	}
	return 0
}

//export IncusGetState
func IncusGetState(name *C.char) C.char {
	str := C.GoString(name)
	container, e := getContainerHandle(name)
	if e < 0 {
		errorG = "Error getting container with the following name: " + str + "; Error code: " + strconv.Itoa(e)
		return -1
	}

	if container.IsActive() {
		return 1
	}
	return 0
}

func main() {
}
