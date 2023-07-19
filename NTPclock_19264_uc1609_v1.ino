/* NTP clock on 2" monochrome display with UC1609c driver
 * original design by niq_ro: https://github.com/tehniq3/ 
 * v.1 - base NTP clock (using info from https://github.com/tehniq3/eink_NTPclock )
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <U8x8lib.h> // included in u8g2 library
#include <SPI.h>

// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
#define HSCK 14 // D5
#define HSDA 13 // D7
#define CS   12 // D6
#define CD    4 // D2
#define RST   5 // D1

U8X8_UC1609_SLG19264_4W_SW_SPI u8x8 ( /* clock=*/  HSCK , /* data=*/  HSDA , /* cs=*/  CS , /* dc=*/  CD , /* reset=*/  RST ) ;   
//U8X8_UC1609_SLG19264_4W_HW_SPI u8x8( /* cs=*/ CS, /* dc=*/ CD, /* reset=*/ RST);  

const long timezoneOffset = 2; // ? hours
const char          *ntpServer  = "pool.ntp.org"; // change it to local NTP server if needed
const unsigned long updateDelay = 900000;         // update time every 15 min
const unsigned long retryDelay  = 5000;           // retry 5 sec later if time query failed

unsigned long tpafisare;
unsigned long tpinfo = 60000;  // 15000 for test in video

unsigned long lastUpdatedTime = updateDelay * -1;
unsigned int  second_prev = 0;
bool          colon_switch = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);

byte DST = 1;
byte DST0;
bool updated;
byte a = 0;

int ora = 20;
int minut = 24;
int secunda = 0;
int zi, zi2, luna, an;
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup()
{
  u8x8.begin() ;
  u8x8.setContrast(150) ;    // contrast 
// u8x8.setInverseFont(1); // invert 
// u8x8.setFlipMode(1); // rotate 180 degrees 
  u8x8.clear();
   
   //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    wifiManager.autoConnect("AutoConnectAP");
      
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    delay(500);

timeClient.setTimeOffset((timezoneOffset + DST)*3600);
timeClient.begin();
}


void loop() {
   if (WiFi.status() == WL_CONNECTED && millis() - lastUpdatedTime >= updateDelay) {
    updated = timeClient.update();
    if (updated) {
      Serial.println("NTP time updated.");
      getYear();
      getMonth();
      getDay();
      zi2 = timeClient.getDay();
      lastUpdatedTime = millis();
    } else {
      Serial.println("Failed to update time. Waiting for retry...");
      lastUpdatedTime = millis() - updateDelay + retryDelay;
    }
  } else {
    if (WiFi.status() != WL_CONNECTED) Serial.println("WiFi disconnected!");
  }
 
unsigned long t = millis();

ora = timeClient.getHours();
minut = timeClient.getMinutes();
secunda = timeClient.getSeconds();

ceas();
delay(500);
}  // end main loop


void ceas()
{
 u8x8. setFont (u8x8_font_inb33_3x6_n);
 u8x8.setCursor(0,2);
 u8x8.print(ora/10);
 u8x8.print(ora%10);
 u8x8.print(":");
 u8x8.print(minut/10);
 u8x8.print(minut%10);
 u8x8.print(":");
 u8x8.print(secunda/10);
 u8x8.print(secunda%10);
    

 u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
 u8x8.setCursor(2,0);
    u8x8.print(zi/10);
    u8x8.print(zi%10);
    u8x8.print(".");
    u8x8.print(luna/10);
    u8x8.print(luna%10);
    u8x8.print(".");
    u8x8.print(an);    

u8x8.setFont(u8x8_font_chroma48medium8_r);
u8x8.drawString (2,7, "www.arduinotehniq.com");
}


 void getYear() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  an = ti->tm_year + 1900;
  Serial.print("year = ");
  Serial.println(an);
}

void getMonth() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  luna = ti->tm_mon + 1;
  Serial.print("month = ");
  Serial.println(luna);
}

void getDay() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  zi = ti->tm_mday;
  Serial.print("day = ");
  Serial.println(zi);
}
