#include "TM1637.h"

static int8_t TubeTab[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};//0~9,A,b,C,d,E,F                        
TM1637::TM1637(PinName Clkpin, PinName Datapin)
{
    this -> Clkpin = Clkpin;
    this -> Datapin = Datapin;
}

void TM1637::init(void)
{
  clearDisplay();
}

void TM1637::writeByte(int8_t wr_data)
{
  uint8_t i,count1;   
  DigitalOut D_pin(Datapin);
  DigitalOut C_pin(Clkpin);
  for(i=0;i<8;i++)        //sent 8bit data
  {
    C_pin = 0;
    if(wr_data & 0x01)D_pin = 1;//LSB first
    else D_pin = 0;
    wr_data >>= 1;    
    C_pin = 1;     
  }  
  C_pin = 0;
  D_pin = 1;
  C_pin = 1;
  DigitalIn D_pin1(Datapin);
  while(D_pin1)    
  { 
    count1 +=1;
    if(count1 == 200)//
    {
     DigitalOut D_pin2(Datapin);
     D_pin2 = 0;
     count1 =0;
    }
    DigitalIn D_pin3(Datapin);
  }
  DigitalOut D_pin4(Datapin); 
}
//send start signal to TM1637
void TM1637::start(void)
{
  DigitalOut D_pin(Datapin);
  DigitalOut C_pin(Clkpin);
  C_pin = 1;
  D_pin = 1;
  D_pin = 0;
  C_pin = 0;
} 
//End of transmission
void TM1637::stop(void)
{
  DigitalOut D_pin(Datapin);
  DigitalOut C_pin(Clkpin);
  C_pin = 0;
  D_pin = 0;
  C_pin = 1;
  D_pin = 1;
}
//display function.Write to full-screen.
void TM1637::display(int8_t DispData[])
{
  int8_t SegData[4];
  uint8_t i;
  for(i = 0;i < 4;i ++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(Cmd_SetAddr);//
  for(i=0;i < 4;i ++)
  {
    writeByte(SegData[i]);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}
//******************************************
void TM1637::display(uint8_t BitAddr,int8_t DispData)
{
  int8_t SegData;
  SegData = coding(DispData);
  start();          //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(BitAddr|0xc0);//
  writeByte(SegData);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

void TM1637::clearDisplay(void)
{
  display(0x00,0x7f);
  display(0x01,0x7f);
  display(0x02,0x7f);
  display(0x03,0x7f);  
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(bool PointFlag)
{
  _PointFlag = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  for(uint8_t i = 0;i < 4;i ++)
  {
    if(DispData[i] == 0x7f)DispData[i] = 0x00;
    else DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t TM1637::coding(int8_t DispData)
{
  uint8_t PointData;
  if(_PointFlag == POINT_ON)PointData = 0x80;
  else PointData = 0; 
  if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
  else DispData = TubeTab[DispData] + PointData;
  return DispData;
}

