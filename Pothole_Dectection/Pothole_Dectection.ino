#include <ESP8266WiFi.h>
#include "IFTTTWebhook.h"
#include <SoftwareSerial.h> 
#include <TinyGPS++.h>
void getlocation();
long duration;
int distance;
char lati[10];
char lon[10];
int mindist=3;
double x,y;
TinyGPSPlus gps;
SoftwareSerial SoftSerial(D7,D8); //GPS MODULE RECEIVER AND TRANSMITTER PINS RESPECTIVELY
#define WIFI_SSID "XXXXXXXXXX"//USERNAME OF THE WIFI
#define  WIFI_PASSWORD "XXXXXXXXXXXX"//PASSWORD IF PRESENT
const int trigPin = D0;
const int echoPin = D1;

#define IFTTT_API_KEY "XXXXXXXXXXXXXXXXXXXXXXXX"//API Key of IFTTT
#define IFTTT_EVENT_NAME "YOUR EVENT NAME FROM IFTTT"
#define IFTTT_FINGERPRINT "AA:75:CB:41:2E:D5:F9:97:FF:5D:A0:8B:7D:AC:12:21:08:4B:00:8C"//FINGERPRINT OF IFTTT(DEFAULT)

void setup() {
  
  SoftSerial.begin(9600);//INITIALIZING SERIAL PORT FOR THE GPS MODULE 
  Serial.begin(9600);
  Serial.println("RUNNING");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);

  //CONNECTING TO WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void loop() {
  getlocation();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;//FINDING THE DISTANCE OF THE OBSTACLE
  
  Serial.println(distance);
  if(distance>mindist)
  {
    double z=distance-mindist;
    char depth[10];
    Serial.println("distance>3");
      dtostrf(x, 4, 4, lati);
      dtostrf(y, 4, 4, lon);
      dtostrf(z, 4, 4, depth);
      Serial.println(lati);
      Serial.println(lon);
      IFTTTWebhook wh(IFTTT_API_KEY, IFTTT_EVENT_NAME,IFTTT_FINGERPRINT);
      wh.trigger(lati,lon,depth);
      Serial.println("Triggered");
  }
}
//GETTING THE LOCATION FROM THE GPS MODULE
void getlocation()
{
  while (SoftSerial.available() > 0)
   gps.encode(SoftSerial.read());
   if (gps.location.isUpdated())
    {
      x=gps.location.lat();
      y=gps.location.lng();
      Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
      Serial.print("  LNG="); Serial.println(gps.location.lng(), 6);
    }
}

