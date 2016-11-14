/*
 * (C) The University of Kent and Simon Cooksey 2015.
 * Proddled a lot by Fred
 */
 
#include "mbed.h"
#include <inttypes.h>
 
#include "dht22.h"
 
/*
 * The DHT22 uses a 1 wire interface, sending 1's and 0s by varying the length
 * of the HIGH time on the signal pin.
 */
 
/* delay for approx 2 microseconds */
void DHT22::wait_2us (void)
{
    int i;
    
    /* 16 * 14 nops, plus loopend */
    for (i=0; i<16; i++) {
        __asm__ __volatile__ ("         \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n" \
            "   nop                     \n");
 
    }
    return;
}
 
 
/* ensure the comms pin is set for input */
void DHT22::setinput (void)
{
    if (!isinput) {
        dht22_s.input ();
        isinput = 1;
    }
}
 
/* ensure the comms pin is set for output */
void DHT22::setoutput (void)
{
    if (isinput) {
        dht22_s.output ();
        isinput = 0;
    }
}
 
 
int DHT22::wait_for_level (int lvl, const int max)
{
    int time;
 
    setinput ();
    
    for (time=0; ((max == -1) || (time < max)) && (dht22_s != lvl); time += 2) {
        wait_2us ();
    }
    if ((max > 0) && (time >= max)) {
        /* timed out */
        return -1;
    }
    
    if (time) {
        /* means we saw a transition, so let it settle */
        wait_2us ();
        time += 2;
    }
    
    return time;            
}
 
 
/*
 * Send a start bit to the DHT22
 */
void DHT22::send_start (void)
{
    int dly;
    
    //__disable_irq ();
    setoutput ();
    dht22_s = 0;
    /* drag low for 1ms */
    for (dly=0; dly<(DHT22_START_BIT_TIME >> 1); dly++) {
        wait_2us ();
    }
    // wait_us (DHT22_START_BIT_TIME);
    dht22_s = 1;
    setinput ();
    //__enable_irq ();
}
 
/*
 * Wait for the DHT22 to send the start bit ACK, after this we can read data.
 */
int DHT22::wait_start (void)
{
    /* level should be 1 */
    if (dht22_s == 0) {
        return -1;
    }
    if (wait_for_level (0, 500) < 0) {
        /* should respond in 20-200 us, it didn't in 500 */
        return -2;
    }
    if (wait_for_level (1, 100) < 0) {
        /* only for 80 us (max 85 in datasheet) */
        return -3;
    }
    if (wait_for_level (0, 100) < 0) {
        /* only for 80 us (max 85 in datasheet) */
        return -4;
    }
    /* at this point we're about to start seeing the MSB of data [39-0] */
    
    return 0;    
}
 
/*
 * reads 8 bits of data, returns value [0-255] on success, -1 on error (timeout)
 */
int DHT22::read_byte (void)
{
    int d, bit;
    int v = 0;
    
    /* should be zero already */
    if (dht22_s == 1) {
        return -1;
    }
    
    for (bit=7; bit>=0; bit--) {
        /* expect it to stay low for 50us (max 55) */
        if (wait_for_level (1, 100) < 0) {
            /* timed out after 100us */
            return -2;
        }
 
 
        d = wait_for_level (0, 100);
        if (d < 0) {
            /* timed out after 100us */
            return -3;
        }
 
 
        if (d > DHT22_SIGNAL_HIGH_LOW_BOUNDARY) {
            v |= (1 << bit);
#ifdef DEBUG_DHT22
            debug = 1;
        } else {
            debug = 0;
#endif
        }
 
    }
    return v;
}
 
 
/*
 * Reads a packet of DHT22 data.
 *
 * Param data: the packet to fill.  returns 0 on success, < 0 on error (timeout of some kind)
 */
int DHT22::read (DHT22_data_t *data)
{
    uint8_t buf[5];
    uint16_t u_hum, u_tmp;
    int i;
 
#ifdef DEBUG_DHT22
    debug = 0;
#endif
    __disable_irq ();
    // Send start bits
    send_start();
 
    if (wait_start () < 0) {
        __enable_irq ();
        return -1;              /* timed out waiting for start response */
    }
    /* read 40 bits worth -- MSB first */
    for (i=4; i>=0; i--) {
        int v = read_byte ();
        
        if (v < 0) {
            /* timed out waiting for data */
            __enable_irq ();
            return -2;
        }
        buf[i] = (uint8_t)v;
    }
 
    /* yay, got here! -- sensor should release the wire in 50us (max 55) */
    if (wait_for_level (1, 100) < 0) {
        /* timed out at this point, but still fill the buffer */
        i = -3;
    } else {
        i = 0;
    }
    __enable_irq ();
    
    /* unscramble */
    u_hum = ((uint16_t)buf[4] << 8) | buf[3];
    u_tmp = ((uint16_t)buf[2] << 8) | buf[1];
    
    if (u_hum & 0x8000) {
        data->humidity = 1 - (int)(u_hum ^ 0x8000);
    } else {
        data->humidity = (int)u_hum;
    }
    if (u_tmp & 0x8000) {
        data->temp = 1 - (int)(u_tmp ^ 0x8000);
    } else {
        data->temp = (int)u_tmp;
    }
 
    data->checksum = buf[0];
 
#ifdef DEBUG_DHT22
    debug = 0;
#endif
    
    return i;
}
 
