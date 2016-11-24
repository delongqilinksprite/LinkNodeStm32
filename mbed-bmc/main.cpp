#include "mbed.h"

DigitalOut led1(D4);


int main() {
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}

  