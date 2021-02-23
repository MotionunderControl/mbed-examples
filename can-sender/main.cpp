#include "mbed.h"

DigitalOut led1(PC_13);         // Onboard LED
CAN can(PB_8, PB_9, 250000);    // Onboard CAN channel connected to TI SN65HVD230 transceiver
Ticker ticker;                  // Ticker with microsecond resolution
Timer timer;                    // Timer with microsecond resolution

// Global data
char can_data[8];
uint32_t counter = 0;
uint32_t t_send;

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
    counter++;
    // Construct a message with counter and send timestamp
    can_data[0] = (counter >> 24) & 0xFF;
    can_data[1] = (counter >> 16) & 0xFF;
    can_data[2] = (counter >> 8) & 0xFF;
    can_data[3] = counter & 0xFF;
    t_send = timer.elapsed_time().count();
    can_data[4] = (t_send >> 24) & 0xFF;
    can_data[5] = (t_send >> 16) & 0xFF;
    can_data[6] = (t_send >> 8) & 0xFF;
    can_data[7] = t_send & 0xFF;
    can.write(CANMessage(0x80, can_data, 8));
}

int main()
{
    printf("This is a CAN send example on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    timer.start();
    ticker.attach(&onTick,100ms);      // periodically call the onTick function

    while (true)
    {
        sleep();    // sleep between the onTick calls
    }
}
