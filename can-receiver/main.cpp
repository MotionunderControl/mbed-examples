#include "mbed.h"

DigitalOut led1(PC_13);         // Onboard LED
CAN can(PB_8, PB_9, 250000);    // Onboard CAN channel connected to TI SN65HVD230 transceiver
Timer timer;                    // Microsecond timer

CANMessage msg;
uint32_t counter_first, counter = 0;    // Number of received CAN messages
chrono::microseconds t_rcv, t_rcv_prev, t_rcv_delta, t_rcv_avg;     // Receive times
uint32_t t_msg_prev, t_msg_delta;       // Message times
chrono::microseconds t_msg, t_msg_avg;  // Message time statistics
#define HIST_WIDTH_US 20us
#define HIST_SIZE 17
#define HIST_MID 8      // (SIZE-1)/2
#define HIST_MIN -HIST_MID*HIST_WIDTH_US
#define HIST_MAX HIST_MID*HIST_WIDTH_US
uint32_t hist_dev[HIST_SIZE] = { 0 };  // histogram with time deviations
uint16_t msg_overflow_cnt = 0;

// Called when new CAN message received
void onCANReceive(void) {
    if (can.read(msg)) {
        t_rcv = timer.elapsed_time();
        if (msg.id == 0x80 and msg.len == 8) {
            // handle message contents: counter and timestamp
            counter = ((uint32_t)msg.data[0] << 24) + ((uint32_t)msg.data[1] << 16) + ((uint32_t)msg.data[2] << 8) + msg.data[3];
            uint32_t t_msg_new = ((uint32_t)msg.data[4] << 24) + ((uint32_t)msg.data[5] << 16) + ((uint32_t)msg.data[6] << 8) + msg.data[7];

            led1 = counter % 2; // toggle LED

            if (counter == 1 or t_rcv == 0us) { // first message (or sender reset)
                timer.reset();  // Reset and start microsecond timer
                timer.start();
                t_msg = chrono::microseconds(0);
                counter_first = counter;
                msg_overflow_cnt = 0;
                for (unsigned int i=0; i < HIST_SIZE; i++) hist_dev[i] = 0;    // Clear histogram
            } else {
                // Receive time updates
                t_rcv_delta = t_rcv - t_rcv_prev;
                t_rcv_avg = t_rcv / (counter - counter_first);

                // Message time updates
                if (t_msg_new < t_msg_prev) msg_overflow_cnt++;
                t_msg_delta = t_msg_new - t_msg_prev;
                t_msg += chrono::microseconds(t_msg_delta);
                t_msg_avg = t_msg / (counter - counter_first);

                // Fill histogram
                chrono::microseconds us_dev = (t_rcv_delta - t_rcv_avg);
                if (us_dev < HIST_MIN) us_dev = HIST_MIN;
                if (us_dev > HIST_MAX) us_dev = HIST_MAX;
                int i = HIST_MID + (us_dev / HIST_WIDTH_US);
                hist_dev[i]++;
            }
            t_rcv_prev = t_rcv;
            t_msg_prev = t_msg_new;
        }
    }
}

void show_stats()
{
    int t_ms = Kernel::Clock::now().time_since_epoch().count();
    printf("---\nStatistics at: %d.%03d s\n", t_ms / 1000, t_ms % 1000);
    printf("Message cnt:\t%d\n", counter);
    printf("Average over:\t%d messages\tMsg time overflows:\t%d\n", counter - counter_first, msg_overflow_cnt);
    printf("T delta\tReceive:\t%lld.%03lld ms\tMessage:\t%d.%03d ms\n", t_rcv_delta.count() / 1000, t_rcv_delta.count() % 1000, t_msg_delta / 1000, t_msg_delta % 1000);
    printf("T total\tReceive:\t%lld.%03lld ms\tMessage:\t%lld.%03lld ms\n", t_rcv.count() / 1000, t_rcv.count() % 1000, t_msg.count() / 1000, t_msg.count() % 1000);
    printf("T avg\tReceive:\t%lld.%03lld ms\tMessage:\t%lld.%03lld ms\n", t_rcv_avg.count() / 1000, t_rcv_avg.count() % 1000, t_msg_avg.count() / 1000, t_msg_avg.count() % 1000);

    float clock_diff_pct = 100*((t_rcv.count() / (float)t_msg.count()) - 1.0);
    int clock_diff = clock_diff_pct * 1000;
    if (t_msg > 0us) printf("Clock diff:\t%d.%05d %%\n", clock_diff / 1000, clock_diff % 1000);
    printf("Hist\t");
    for (unsigned int i=0; i < HIST_SIZE; i++) {
        printf("|%5d", hist_dev[i]);
    }
    printf("|\n");
    printf("Dev[us]\t %5lld\t\t..\t\t..\t\t %5d\t\t..\t\t..\t\t %5lld\n", HIST_MIN.count(), 0, HIST_MAX.count());
    printf("\n");
}

int main()
{
    printf("This is a CAN receive example on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    for (int i=0; i < 15; i++) hist_dev[i] = 0;
 
    can.reset();    // Reset to flush read buffer
    can.attach(&onCANReceive, CAN::RxIrq);  // called when new CAN message received

    while (true)
    {
        ThisThread::sleep_for(2s);
        show_stats();
    }
}
