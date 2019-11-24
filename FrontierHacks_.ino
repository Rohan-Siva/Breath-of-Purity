#include <WiFi101.h>
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>
#define AIO_SERVER "io.adafruit.com" 
#define AIO_SERVERPORT 1883 
#define AIO_USERNAME "RohanSiva"
#define AIO_KEY "daff716f2189437082ce3bebb4bf377a"
#define WLAN_SSID "MakeSchool" 
#define WLAN_PASS "applynow" 
#define DHTTYPE DHT22
#define DHTPIN 7 
char b[14]={"Fire Detected"};
char c[17]={"No Fire Detected"};
float hum3;
float temp3;
static int temp = 0; 
static int tempPrev = 0;
const int sensorMin = 0;
const int sensorMax = 1024;
int chk;
WiFiClient client; 
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/anduinoTemp"); 

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.begin(115200);   
  delay(1000); 
  connectToWiFi(); 
  connectToAdafruit();

}

void loop() {
  hum3 = dht.readHumidity();
  temp3= (dht.readTemperature()*1.8)+32;
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum3);
  Serial.print(" %, Temp: ");
  Serial.print(temp3);
  Serial.println(" Fahrenheit");
  int sensorValue = analogRead(0); 
  int sensorReading = analogRead(1);
  Serial.print("The Air Quality is: ");      
  Serial.print(sensorValue, DEC); 
  Serial.print(" PPM");
  Serial.println();
  delay(1000);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);  

  switch (range) {  // switch loop using the range given from analog input 0
  case 0:    // A fire closer than 1.5 feet away.
    //temperature.publish((char*)b);
    chk=0;
    delay(9000);
    break;
  case 1:    // A fire between 1-3 feet away.
    chk=1;
    break;
  case 2:    // No fire detected.
    //temperature.publish((char*) c);
    chk=2;
    Serial.println("No Infrared Detected");
    break;
  }
  switch(chk){
    case 0:
      if((temp3>65) & (hum3<40)){
         temperature.publish((char*)b);
      }else{
        temperature.publish((char*)c);
      }
      break;
    case 2:
      temperature.publish((char*)b);
      break;
  }

}
  
  
  
void connect() { 
 Serial.print(F("Connecting to Adafruit IO... ")); 
 int8_t ret; 
 while ((ret = mqtt.connect()) != 0) { 
   switch (ret) { 
     case 1: Serial.println(F("Wrong protocol")); break; 
     case 2: Serial.println(F("ID rejected")); break; 
     case 3: Serial.println(F("Server unavail")); break; 
     case 4: Serial.println(F("Bad user/pass")); break; 
     case 5: Serial.println(F("Not authed")); break; 
     case 6: Serial.println(F("Failed to subscribe")); break; 
     default: Serial.println(F("Connection failed")); break; 
   } 
   if(ret >= 0) 
     mqtt.disconnect(); 
   Serial.println(F("Retrying connection...")); 
   delay(5000); 
 } 
 Serial.println(F("Adafruit IO Connected!")); 
}

void connectToWiFi(){ 
 // Connect to WiFi access point. 
 delay(10); 
 Serial.print(F("Connecting to ")); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print(F(".")); 
 } 
 Serial.println(F("WiFi connected!")); 
} 
void connectToAdafruit() { 
   // connect to adafruit io 
 connect(); 
}
