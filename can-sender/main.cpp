#include "mbed.h"

DigitalOut led1(PC_13);     // Onboard LED
CAN can(PB_8, PB_9);        // Onboard CAN channel connected to TI SN65HVD230 transceiver
Ticker t;                   // Ticker with microsecond resolution

// Serial console output is sent by default to UART2 (PA_2, PA_3)
//BufferedSerial serial(PA_2, PA_3, 115200);
// Redirect console output to serial port
/*FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial;
}*/

// Periodically called
void onTick(void) {
    led1 = !led1;
    if (led1) can.write(CANMessage(0x80));
    else can.write(CANMessage(0x81));
}

int main()
{
    printf("This is a CAN send example on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    t.attach(&onTick,0.2);      // periodically call the onTick function

    while (true)
    {
        sleep();    // sleep between the onTick calls
    }
}
