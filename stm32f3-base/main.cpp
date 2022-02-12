#include "mbed.h"
#include "USBSerial.h"

DigitalOut led1(LED1);      // Onboard LED
DigitalOut out1(PA_0);      // Digital Output
//USBSerial serial(false);    // Non-blocking
//CAN can(PB_8, PB_9);        // Onboard CAN channel connected to TI SN65HVD230 transceiver
Ticker t;                   // Ticker with microsecond resolution

// Redirect console output to usb-serial port
/*FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial;
}*/

// Periodically called
void onTick(void) {
    led1 = !led1;
    out1 = !out1;
}

int main()
{
    t.attach(&onTick,500ms);      // periodically call the onTick function

    printf("STM32F3base example application on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    while (true)
    {
        printf("Ping\n");
        ThisThread::sleep_for(1s);
    }
}
