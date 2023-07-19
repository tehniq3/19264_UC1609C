// based on info from http://rcl-radio.ru/?p=129222
/*
 * v1 - pseudo clock with Arduino Nano & 2" monocherome display (192x64 - UC1609C driver)
 */
#include <U8x8lib.h> // included in u8g2 library
#include <SPI.h>

//U8X8_UC1609_SLG19264_4W_SW_SPI u8x8 ( /* clock=*/  13 , /* data=*/  11 , /* cs=*/  8 , /* dc=*/  10 , /* reset=*/  9 ) ;   
U8X8_UC1609_SLG19264_4W_HW_SPI u8x8( /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  

int ora = 19;
int minut = 35;
int secunda = 47;

int previoussec = 60;
int zo, uo;
int zm, um;
int zs, us;

void setup ( ) { 
  u8x8. begin ( ) ;
  u8x8. setContrast ( 150 ) ;    // contrast 
// u8x8.setInverseFont(1); // invert 
// u8x8.setFlipMode(1); // rotate 180 degrees 
  u8x8. clear ( ) ;
}
 
void loop ( ) { 

  u8x8.setFont(u8x8_font_px437wyse700a_2x2_r) ;
  u8x8.drawString(2,0, "19.07.2023" );

  if (millis()/1000 != previoussec)
  {
    previoussec = millis()/1000;
    secunda++;
  } 
  if (secunda > 59)
  {
    secunda = 0;
    minut++;
  }
  if (minut > 59)
  {
    minut = 0;
    ora++;
  }
  if (ora > 23)
  {
    ora = 0;
  } 
zo = ora/10;
uo = ora%10;
zm = minut/10;
um = minut%10;
zs = secunda/10;
us = secunda%10;;

u8x8. setFont (u8x8_font_inb33_3x6_n);
 u8x8.setCursor(0,2);
 u8x8.print(zo);
 u8x8.print(uo);
 u8x8.print(":");
 u8x8.print(zm);
 u8x8.print(um);
 u8x8.print(":");
 u8x8.print(zs);
 u8x8.print(us);

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString (2,7, "www.arduinotehniq.com");
  delay(1000);
  u8x8.clear();
}
