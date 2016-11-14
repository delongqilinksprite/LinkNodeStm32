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

#include "Thumb.h"

THUMB::THUMB(PinName x_pin,PinName y_pin)
{
    this -> x_pin = x_pin;
    this -> y_pin = y_pin;
}


Val THUMB::get_data()
{
    Val dat;
    long x_tmp,y_tmp;
    AnalogIn d1(x_pin);
	for(int i = 0 ; i < 10 ; i ++)
	{
		x_tmp += d1.read_u16();
		wait_ms(2);
	}
    AnalogIn d2(y_pin);
	for(int j = 0 ; j < 10 ; j ++)
	{
		y_tmp += d2.read_u16();
		wait_ms(2);
	}
    dat.x = x_tmp/10;
    dat.y = y_tmp/10;
	return dat;
}
