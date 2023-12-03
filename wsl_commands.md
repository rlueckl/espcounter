Quick Reference for WSL (2) / Ubuntu 22.04.3 LTS
------------------------------------------------

Upgrade to WSL 2:
* `wsl -l -v`
* `wsl --set-default-version 2`
* `wsl --set-version [distro_name] [wsl_version_number]`
* Reboot

Upgrade to at least Ubuntu 20.04 and Kernel 5.15:
* `do-release-upgrade`
* If it fails, remove snapd: `sudo apt purge snapd`

Install usbipd:
* Guide: [Connecting USB devices to WSL](https://devblogs.microsoft.com/commandline/connecting-usb-devices-to-wsl/)
* Download: [usbipd-win/releases](https://github.com/dorssel/usbipd-win/releases)
* Reboot
* `sudo apt install linux-tools-[version]-generic hwdata`
* `sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/[version]-generic/usbip 20`

Attach device from **admin** command line:
* `usbipd wsl list`
* `usbipd wsl attach --busid [bus_id]`

Install tools:
* `sudo apt install python3-pip picocom`
* `pip install esptool`

If `esptool` doesn't work ("Could not open /dev/ttyUSB0, the port doesn't exist"):
* `sudo chmod a+rw /dev/ttyUSB0`

Connect to MicroPython REPL:
* `picocom /dev/ttyUSB0 -b115200`
* Exit Picocom with <kbd>Ctrl</kbd>+<kbd>a</kbd>, then <kbd>Ctrl</kbd>+<kbd>x</kbd>
