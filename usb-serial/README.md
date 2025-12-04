# USB serial

USB serial example application for the STM32 bluepill using the mbed framework. A custom target was made to make the bluepill work with mbed OS 6.17.0.

# Howto

Project can be built using mbed Studio or from the command line.

## MBED-CLI 1

Target dependencies

`mbed add https://github.com/vznncv/TARGET_BLACKPILL_F401CC.git`
`mbed add https://github.com/vznncv/TARGET_BLUEPILL_F103C8.git`

Copy/combine `custom_target.json` from TARGET folder

Obtain the mbed-os folder once with

`mbed deploy`

Set default target and toolchain

`mbed target TARGET_BLUEPILL_F103C8`
`mbed target TARGET_BLACKPILL_F401CC`
`mbed target TARGET_BLACKPILL_F411CE`

`mbed toolchain GCC_ARM`

Then compile the project with

`mbed compile`

## MBED-CLI 2

`mbed-tools deploy`

The compile the project with

`mbed-tools compile  -t GCC_ARM -m BLUEPILL`

Flashing the binary to the target and debugging can be done with ST-LINK v2
