/**
 * Low level infrared transmission
 * This library provides a low level interface to send IR commands. in principle
 * other higher level IR type remotes could be build on top of this. I wrote
 * this code to send IR commands to a Nikon camera, which it works rather well
 * for. 
 *
 * Copyright (C) 2010 Ali Saidi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
/**
 * Copyright 2014 Nordic Semiconductor
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 */


#include "mbed.h"
#include "IRSender.h"

bool IRSender::irSeq(unsigned freq, unsigned len, const unsigned *data)
{
    // TODO: implement a lock or semaphore here
    if (_inUse) {
        return false;
    }
    _inUse = true;

    int pos = 0;
    senderPin.write(0.0);
    senderPin.period_us(freq);

    int times[len];
    for (int i=0; i<len; i++) {
        times[i] = 0;
    }

    int bit = 1;
    LOG("Start\n");
    senderPin.write(0.5);
    timer.start();

    int time = timer.read_us();
    int old_time = time;
    int dt = 0;

    while (pos < len) {
        // TODO: Handle timer overflow
        // Consider using GPIOTE
        old_time = time;
        time = timer.read_us();
        dt += time - old_time;

        while (dt > data[pos] && pos < len) {
            dt -= data[pos];
            bit = !bit;
            senderPin.write(0.5 * bit);
            times[pos] = dt;
            pos++;
        }
    }

    timer.stop();
    senderPin.write(0);
    LOG("Success, wrote %i/%i\n", pos, len);
    LOG("Miss timings: %i", times[0]);
    for (int i=1; i<len; i++) {
        APPEND(", %i", times[i]);
    }
    APPEND("]\n");
    _inUse = false;
    return true;
}
