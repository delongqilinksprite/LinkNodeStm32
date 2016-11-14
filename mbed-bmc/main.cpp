#include "mbed.h"
#include "Thumb.h"

Serial pc(SERIAL_TX,SERIAL_RX);
THUMB Thumb(A0,A1);

int main() 
{
    Val data;
    pc.baud(115200);
    while(1)
    {
        data = Thumb.get_data();
        pc.printf("X value is :%d\n",data.x);
        pc.printf("Y value is :%d\n",data.y);
        wait(1);
    }
}

