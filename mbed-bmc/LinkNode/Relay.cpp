/*******************************************************************
*author:Derron from LinkSprite
*date:2016.11.09
*attation: 
*PINMAP:
*   A0          = PA_4,
*   A1          = PA_5,
*   A2          = PA_6,
*   A3          = PA_7,
*   A4          = PB_0,
*   A5          = PB_1,
*   D0          = PA_0,
*   D1          = PA_1,
*   D2          = PA_2,
*   D3          = PA_3,
*   D4          = PA_8,
*   D5          = PB_8,
*   D6          = PB_9,
*   D7          = PB_15,
*   D8          = PB_14,
*   D9          = PB_13,
********************************************************************/

#include "Relay.h"

RELAY::RELAY(PinName relay_pin)
{
	this->relay_pin = relay_pin;
}

int RELAY::On()
{
    DigitalOut relay(relay_pin);
	relay = 1;
	return 0;
}

int RELAY::Off()
{
    DigitalOut relay(relay_pin);
	relay = 0;
	return 0;
}


