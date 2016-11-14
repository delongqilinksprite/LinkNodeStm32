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

#ifndef __IRSENDER__H__
#define __IRSENDER__H__

#include "mbed.h"

#define DEBUG 0

#if DEBUG
    #define LOG(fmt, ...) do {  __IRSENDER__irLogger.printf("[IRSender] "); __IRSENDER__irLogger.printf(fmt, ##__VA_ARGS__); } while(0)
    #define APPEND(fmt, ...) do { __IRSENDER__irLogger.printf(fmt, ##__VA_ARGS__); } while(0)
    static Serial __IRSENDER__irLogger(USBTX, USBRX);
#else
    #define LOG(fmt, ...)
    #define APPEND(fmt, ...)
#endif

/** Small library to drive the pin of an IR LED.
 *  Send the frequency of the signal, the length
 *  of the data and the duration of on and off
 *  in ms.
 */

class IRSender {
    private:
        bool _inUse;
        PwmOut senderPin;
        Timer timer;

    public:
        IRSender(PinName pin) : _inUse(false), senderPin(pin) {}
        bool irSeq(unsigned freq, unsigned len, const unsigned *data);
};

#endif // __IRSENDER__H__
