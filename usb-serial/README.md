# USB serial

USB serial example application for the STM32 bluepill using the mbed framework. A custom target was made to make the bluepill work with mbed OS 6.6.0.

# Howto

Project can be built using mbed Studio or from the command line.

## MBED-CLI 1

Obtain the mbed-os folder once with

`mbed deploy`

Then compile the project with

`mbed compile -t GCC_ARM -m BLUEPILL`

Flashing the binary to the target and debugging can be done with ST-LINK v2
