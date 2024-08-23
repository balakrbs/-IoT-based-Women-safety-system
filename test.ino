//ws2247@yopmail.com
//mayugamemb@gmail.com

#include <EveryTimer.h>
#define PERIOD_MS 1000

EveryTimer timer;
bool active = true;

#include <LCD_I2C.h>

LCD_I2C lcd(0x27);

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BLYNK_TEMPLATE_ID "TMPL3mjeMgPXw"
#define BLYNK_TEMPLATE_NAME "GPS TRACKING"
#define BLYNK_AUTH_TOKEN "dCSS3KVOy7KJ5-UlkA2hgYlpG3lwFuzV"
char auth[] = BLYNK_AUTH_TOKEN;

const char* ssid     = "K r b s"; 
const char* password = "krbs#124"; 

const char* host = "maker.ifttt.com";

const int PulseWire = A0;      
unsigned int myBPM,bpm ,p,X,sec,count; 
int t,h;

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(D3, D4);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;      

volatile float minutes, seconds;
volatile int degree, secs, mins;
float latn,longn;


     double lat_val, lng_val, alt_m_val;
        uint8_t hr_val, min_val, sec_val;
        bool loc_valid, alt_valid, time_valid;

#define btn D6
String s = "www.google.com/maps/dir/";
void setup()
{
  Serial.begin(9600);
  pinMode(btn,INPUT_PULLUP);
    lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                 // this stop the library(LCD_I2C) from calling Wire.begin()
    lcd.backlight();
 
  
  // Call the callback every 1000 milliseconds
 dht.begin();
      pinMode(PulseWire,INPUT);        
  Blynk.begin(auth, ssid, password, "blynk.cloud", 80);
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
}

void loop()
{
   t = dht.readTemperature();
    lcd.setCursor(0,1);lcd.print("T:");
    if(t <= 9){lcd.print("0");lcd.print(t);}
    else if(t <= 99){lcd.print(t);}



   bpm=analogRead(PulseWire);
 
if(bpm<=1000){
}
else {bpm=0;}


 lcd.setCursor(0,0);
 lcd.print("HB :");
 if(bpm<=9){lcd.print("00"); lcd.print(bpm);}
 else if(bpm<=99){lcd.print("0"); lcd.print(bpm); delay(400);}
 else if(bpm<=999){lcd.print(""); lcd.print(bpm);}





       smartDelay(200); /* Generate precise delay of 1ms */
        unsigned long start;
   
        lat_val = gps.location.lat(); /* Get latitude data */
        loc_valid = gps.location.isValid(); /* Check if valid location data is available */
        lng_val = gps.location.lng(); /* Get longtitude data */
        alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
        alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
        hr_val = gps.time.hour(); /* Get hour */
        min_val = gps.time.minute();  /* Get minutes */
        sec_val = gps.time.second();  /* Get seconds */
        time_valid = gps.time.isValid();  /* Check if valid time data is available */
      
            
        if (!loc_valid)
        {    
                
          Serial.print("Latitude : ");
          Serial.println("***");
          Serial.print("Longitude : ");
          Serial.println("***");
        }
        else
        {
          DegMinSec(lat_val);
       
          DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
        
          longn=(lng_val, 6);
      
        }
        if (!alt_valid)
        {
          
        }
        else
        {
         
        }
        if (!time_valid)
        {
          
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "Time : %02d/%02d/%02d \n", hr_val, min_val, sec_val);
          Serial.print(time_string);    
        }
   

if(!alt_valid){lat_val=10.94191587410371;lng_val=76.90013395621689;}




s = "www.google.com/maps/dir/";
s += String(lat_val, 6);
  s += ",";
  s += String(lng_val, 6);
  s += "/";

  Serial.println(s);
  
  Blynk.virtualWrite(V4,s);
 WiFiClient client; 
      const int httpPort = 80;  
      if (!client.connect(host, httpPort)) 
      {  
         Serial.println("connection failed");  
         return;
      } 
 
         
         if (digitalRead(btn)==LOW || t>=40 || bpm>=110)
        {    

          Blynk.logEvent("msg",s);
          lcd.setCursor(0,0);
          lcd.print(" EMERGENCY KEY");
            lcd.setCursor(0,1);
          lcd.print("   PRESSED");


           String url = "/trigger/gps/json/with/key/p7cRxK2scRfeOZK9LCmTNIqrlc04VgweJ38ZWZiML58"; 
            Serial.print("Requesting URL: ");
            Serial.println(url);                 
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");    
         
         delay(3000);
         lcd.clear();
         }                     
          else
          {
            Serial.println("Correct character not pressed.Try again");            
          }
                 
       
  delay(400);


}




  static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}