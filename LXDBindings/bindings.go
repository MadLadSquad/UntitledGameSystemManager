package main

import (
	"C"
	lxd "github.com/lxc/lxd/client"
	"github.com/lxc/lxd/shared/api"
	_ "sync"
	_ "unsafe"
)

var (
	c      lxd.InstanceServer
	errorG string
	err    error
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

func main() {}
