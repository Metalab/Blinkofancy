/*
Blinkofancy 

overflo
wizard23
amir 
nex
 
*/
 
 
#include "SPI.h"
 
#define VAR 0
 
#define CLEAR_PANEL_PIN 10
// this goes on panel to pin #10
 
#define DATA_PIN 11
// this goes on panel to pin #8
 
#define BLINK_PIN 12
// this goes on panel to pin #6
 
#define CLOCK_PIN 13
// this goes on panel to pin #4
 
#define PANELS 30
#define PANELDATA_SIZE (10*PANELS)
 
uint8_t panelData[PANELDATA_SIZE];
 
void setPixel(int x, int y, int value) {
  int index =  ((y / 9) * 480) +  (x * 10) + (y % 9)+ 1;
  int byteNr = index >> 3; 
  int bitNr = index & 0x7;
 
   if (value) {
     panelData[byteNr] |= 1 << bitNr;
   } else {
     panelData[byteNr] &= ~(1 << bitNr);
   }
}
 
void setAllPixel(uint8_t value)
{
  for (int i = 0; i < PANELDATA_SIZE; i++)
  {
    panelData[i] = value;
  }
}
 
void shiftPixelData()
{
  screen_off();delay(5);
  for (int i = 0; i < PANELDATA_SIZE; i++)
  {
    uint8_t value = panelData[i];
    
    SPDR = panelData[i];
    while(!(SPSR & (1<<SPIF)));
  }delay(5);
  screen_on();
}
 
 
 
void setup()
{
  pinMode(CLEAR_PANEL_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(BLINK_PIN,OUTPUT);
  
  digitalWrite(CLEAR_PANEL_PIN,LOW);
  digitalWrite(DATA_PIN,LOW);
  digitalWrite(CLOCK_PIN,LOW);
  digitalWrite(BLINK_PIN,LOW);
  
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128); // biggest divider there is.
  Serial.begin(115200);   
  blink_off();
}

void screen_off()
{
    digitalWrite(CLEAR_PANEL_PIN,LOW);
}
 
void screen_on()
{
    digitalWrite(CLEAR_PANEL_PIN,HIGH);
}
 
void blink_off()
{
    digitalWrite(BLINK_PIN,LOW);
}
 
void blink_on()
{
    digitalWrite(BLINK_PIN,HIGH);
}
 
 
int pix = 0;

void loop () {

 for(int i = 0; i < 8; ++i) {
 while(Serial.available() < 1) {}
   int r = Serial.read();    
      if(((uint8_t)r )!= 170)
        return;
  }
  
  char pix;
  int hPix = 45;
  int wPix = 48;

  for (int y = 0; y < hPix; ++y)  {
    for (int x = 0; x < (wPix / 8); ++x) {
     while(Serial.available() < 1) { }
      pix = Serial.read();
      if(pix == -1)
        return;
      for(int b = 0; b < 8; ++b) {
         if(((pix >> b) & 0x01) == 1)
           setPixel((x * 8) +( 7 - b),y,1);
         else
           setPixel((x * 8) + (7  - b),y,0);
      }
    }
  }

  shiftPixelData();
}
