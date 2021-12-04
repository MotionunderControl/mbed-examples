# STM32F3base

Example application for the custom STM32F3base module using the mbed framework. A custom target was made to make the target work with mbed OS 6.15.1.

# Howto

Project can be built using mbed Studio or from the command line.

## MBED-CLI 1

Obtain the mbed-os folder once with

`mbed deploy`

Then compile the project with

`mbed compile -t GCC_ARM -m F3BASE`

Flashing the binary to the target and debugging can be done with ST-LINK v3
