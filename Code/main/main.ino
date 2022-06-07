// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include "ThingSpeak.h"
#define DHTPIN 6     //what pin we're connected to                                                                                                              

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21  


#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25


Adafruit_BMP085 bmp;


DHT dht(DHTPIN, DHTTYPE);



int redLed = 12;
int greenLed = 11;
int buzzer = 10;
int smokeA0 = A5;
// Your threshold value
int sensorThres = 400;



#include <SoftwareSerial.h>
#define RX 2
#define TX 3
String AP = "To link";       // AP NAME
String PASS = "32532340"; // AP PASSWORD
String API = "";   // Write Your API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 


void sendCommand(String command, int maxTime, char readReplay[]); 
  
void setup() {

  // esp8266 wifi module init
  
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");



  // dht11 init
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  dht.begin();


  // gas sensor
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);



  Serial.begin(9600);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {
        if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        }  
    }
  }
}


void loop() {


  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
  // Serial.println(hi2);
  // float hi = (9/5) * (hi2 - 32);

  
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print(f);
  Serial.print(" *F\n");
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\n");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *C");



  // gas sensor
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 200);
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    noTone(buzzer);
  }
  delay(100);





   Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    float pressure = (bmp.readPressure()) / 1000.00;
    Serial.print(pressure);
    Serial.println(" kPa");

    Serial.print("Altitude = ");
    float altitude1 = bmp.readAltitude(); 
    Serial.print(altitude1);
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    float pressureSeaLevel = (bmp.readSealevelPressure()) / 1000.00; 
    Serial.print(pressureSeaLevel);
    Serial.println(" Pa");

    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
    Serial.println(" meters");
    
    Serial.println();
    delay(500);



  int sensorsDatas[] = {t, h, hi, analogSensor, pressure, altitude1, pressureSeaLevel};



  Serial.println(sensorsDatas[0]);
  int len = sizeof(sensorsDatas) / sizeof(sensorsDatas[0]);
  Serial.println("length:" + len);
  
  
  for(int i = 1; i <= len; i++) {
    
    
   // valSensor = getSensorData(sensorsData[i]);
   String getData = "GET /update?api_key="+ API +"&"+ "field" + i  +"="+String(sensorsDatas[i-1]);
   Serial.println(getData);
   sendCommand("AT+CIPMUX=1",5,"OK");
   sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
   sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
   esp8266.println(getData);delay(1500);countTrueCommand++;
   sendCommand("AT+CIPCLOSE=0",5,"OK");
   
  }
}




void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
