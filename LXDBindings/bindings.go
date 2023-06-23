package main

import (
	"C"
	"fmt"
	lxd "github.com/lxc/lxd/client"
	"github.com/lxc/lxd/lxc/config"
	"github.com/lxc/lxd/shared/api"
	"github.com/lxc/lxd/shared/version"
	"os"
	"strconv"
	_ "sync"
	_ "unsafe"
)

type internalPulseSocketAppendType uint

const (
	InternalPulseSocketAppendTypeNone        internalPulseSocketAppendType = 1 << 0
	InternalPulseSocketAppendTypeHome        internalPulseSocketAppendType = 1 << 1
	InternalPulseSocketAppendTypeHomeAndNone internalPulseSocketAppendType = InternalPulseSocketAppendTypeNone | InternalPulseSocketAppendTypeHome
)

var (
	c                     lxd.InstanceServer
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

//export LXDGetError
func LXDGetError() *C.char {
	return C.CString(errorG)
}

//export LXDCreateConnection
func LXDCreateConnection() C.char {
	c, err = lxd.ConnectLXDUnix("", nil)
	if err != nil {
		errorG = err.Error()
		return -1
	}
	return 0
}

//export LXDDestroyConnection
func LXDDestroyConnection() {
	c.Disconnect()
}

func LXDGetContainers() ([]api.Container, error) {
	return c.GetContainers()
}

//export LXDStartContainer
func LXDStartContainer(name *C.char) C.char {
	containers, err := LXDGetContainers()
	if err != nil {
		errorG = err.Error()
		return -1
	}

	str := C.GoString(name)

	for _, container := range containers {
		if container.Name == str {
			if !container.IsActive() {
				op, err := c.UpdateContainerState(
					str, api.ContainerStatePut{Action: "start", Timeout: -1}, "")
				if err != nil {
					errorG = err.Error()
					return -1
				}
				op.Wait()
			}
			break
		}
	}

	return 0
}

//export LXDStopContainer
func LXDStopContainer(name *C.char) C.char {
	containers, err := LXDGetContainers()
	if err != nil {
		errorG = err.Error()
		return -1
	}

	str := C.GoString(name)

	for _, container := range containers {
		if container.Name == str {
			if container.IsActive() {
				op, err := c.UpdateContainerState(
					str, api.ContainerStatePut{Action: "stop", Timeout: -1}, "")
				if err != nil {
					errorG = err.Error()
					return -1
				}
				op.Wait()
			}
			break
		}
	}

	return 0
}

//export LXDRestartContainer
func LXDRestartContainer(name *C.char) C.char {
	containers, err := LXDGetContainers()
	if err != nil {
		errorG = err.Error()
		return -1
	}

	str := C.GoString(name)

	for _, container := range containers {
		if container.Name == str {
			if container.IsActive() {
				op, err := c.UpdateContainerState(
					str, api.ContainerStatePut{Action: "restart", Timeout: -1}, "")
				if err != nil {
					errorG = err.Error()
					return -1
				}
				op.Wait()
			}
			break
		}
	}

	return 0
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

//export LXDNewContainer
func LXDNewContainer(name *C.char, alias *C.char) C.char {
	defaultConfig := config.NewConfig("", true)
	defaultConfig.UserAgent = version.UserAgent

	d, err := lxd.ConnectSimpleStreams("https://images.linuxcontainers.org", nil)
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
	op.Wait()

	pulseDir := getPulseSocketLocation()

	if pulseDir == "" {
		errorG = `Couldn't find the PulseAudio socket location. Please set the "PULSE_SERVER" environment variable to a location like "/pulse-native" or "$HOME/pulse-native"`
		return -1
	}

	req := api.ContainersPost{
		Name: C.GoString(name),
		Source: api.ContainerSource{
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

	nop, err := c.CreateContainer(req)
	if err != nil {
		errorG = err.Error()
		return -1
	}
	nop.Wait()

	return LXDStartContainer(name)
}

//export LXDDeleteContainer
func LXDDeleteContainer(name *C.char) C.char {
	containers, err := LXDGetContainers()
	if err != nil {
		errorG = err.Error()
		return -1
	}

	str := C.GoString(name)

	for _, container := range containers {
		if container.Name == str {
			// If active stop container
			if container.IsActive() {
				op, err := c.UpdateContainerState(
					str, api.ContainerStatePut{Action: "stop", Timeout: -1}, "")
				if err != nil {
					errorG = err.Error()
					return -1
				}
				op.Wait()
			}

			op, err := c.DeleteContainer(str)
			if err != nil {
				errorG = err.Error()
				return -1
			}

			op.Wait()
			break
		}
	}

	return 0
}

func main() {
	//LXDCreateConnection()
	//LXDNewContainer(C.CString("tttttttt"), C.CString("archlinux"))
	fmt.Println(getPulseSocketLocation())
}
