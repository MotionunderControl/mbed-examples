#include "mbed.h"

DigitalOut led1(PC_13);     // Onboard LED
CAN can(PB_8, PB_9);        // Onboard CAN channel connected to TI SN65HVD230 transceiver

// Serial console output is sent by default to UART2 (PA_2, PA_3)
//BufferedSerial serial(PA_2, PA_3, 115200);
// Redirect console output to serial port
/*FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial;
}*/

int counter = 0;        // Number of received CAN messages
CANMessage msg;

// Called when new CAN message received
void onCANReceive(void) {
    if (can.read(msg)) {
        ++counter;
        if (msg.id == 0x80) led1 = 1;
        else led1 = 0;
    }
}


int main()
{
    printf("This is a CAN receive example on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

//    can.attach(&onCANReceive, CAN::RxIrq);  // called when new CAN message received

    while (true)
    {
        if (can.read(msg)) {
            printf("Message %d received: %d\n", counter, msg.id);
            if (msg.id == 0x80) led1 = 1;
            else led1 = 0;
        }
        //sleep();
    }
}
