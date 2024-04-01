# Xiao Round display example application

This repository contains a Zephyr application to demonstrate the use of the Seeed Studio [Round display for Xiao][display] with a [Xiao BLE (Sense)][xiao_ble] board.

[display]: https://wiki.seeedstudio.com/get_start_round_display/
[xiao_ble]: https://wiki.seeedstudio.com/XIAO_BLE/

It uses custom drivers for :

- TFT display GC9A01, based off of [Jakob Krantz' work on ZSWatch](https://github.com/jakkra/ZSWatch/blob/main/app/drivers/display/gc9a01/gc9a01.c)
- Touch screen controller CHSC6X, based on the [Seeed Studio Arduino library](https://github.com/Seeed-Studio/Seeed_Arduino_RoundDisplay)

It uses built-in Zephyr drivers for:

- reading/writing to the SD card slot present on the display
- using the PCF8563 RTC present on the display

## Getting started

Before getting started, make sure you have a proper nRF Connect SDK development environment.
Follow the official
[Installation guide](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started.html).

### Initialization

The first step is to initialize the workspace folder (``my-workspace``) where
the ``application`` and all nRF Connect SDK modules will be cloned. Run the following
command:

```shell
# initialize my-workspace for the application (main branch)
west init -m https://github.com/nicogou/xiao-round-display --mr main my-workspace
# update nRF Connect SDK modules
cd my-workspace
west update
```

### Building and running

Build using nrf Connect for VSCode, specifying the correct overlay for the Xiao BLE Sense and the Round Display for Xiao.

Once you have built the application, run the following command to flash it:

```shell
west xiao-flash -p COMx -d Y:
```
Where COMx specifies the serial port the application is hooked on when running, and Y: is the drive that is mounted when the Xiao is in bootloader mode.
This command checks if a Xiao already in bootloader mode is plugged in and ready to receive a UF2 update. If not, it switches it into bootloader mode
using the serial port, and proceeds with the flashing.

### Testing

To execute Twister integration tests, run the following command:

```shell
west twister -T tests --integration
```
