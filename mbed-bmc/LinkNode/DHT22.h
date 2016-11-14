/*
 * (C) The University of Kent and Simon Cooksey 2015.
 */
 
#ifndef __DHT22_h_
#define __DHT22_h_
 
#include "mbed.h"
 
// We'll pick a point to decide if a signal is 1 or 0 from. 
#define DHT22_SIGNAL_HIGH_LOW_BOUNDARY      40   // uS
#define DHT22_START_BIT_TIME                1000  // uS
#define DHT22_START_BIT_RESPONSE            80   // uS
 
#undef DEBUG_DHT22
 
typedef struct {
    int temp;
    int humidity;
    uint8_t checksum;
    char dummy[3];
} DHT22_data_t;
 
class DHT22 {
public:
    DHT22 (PinName pin) : dht22_s (pin)
#ifdef DEBUG_DHT22
                            , debug (PTB19)     /* GROT! -- hardwired for K64F */
#endif
    {
        dht22_s.input ();
        isinput = 1;
    }
    
    int read (DHT22_data_t *ptr);
private:
    DigitalInOut dht22_s;
    int isinput;
#ifdef DEBUG_DHT22
    DigitalOut debug;
#endif
 
    void wait_2us (void);
    void setinput (void);
    void setoutput (void);
    
    int wait_for_level (int lvl, const int max);
    void send_start (void);
    int wait_start (void);
    int read_byte (void);
};
 
#endif // __DHT22_h_
 
