// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 4     //what pin we're connected to                                                                                                              

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21  


DHT dht(DHTPIN, DHTTYPE);





#include <SoftwareSerial.h>
#define RX 2
#define TX 3
String AP = "To link";       // AP NAME
String PASS = "32532340"; // AP PASSWORD
String API = "FEMCDFL9QHLJQBO8";   // Write API KEY
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

  /*
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
  Serial.println(" *F");
  */
  

  sendDataToThinkSpeak();




  int sensorsDatas[] = {t, h, hi};
  

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

 
 
 /*
  
 valSensor = getSensorData(sensorsData[0]);
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");*/
}



String sensorsDataName[] = {"temp", "humidity", "heat_index"};


void sendDataToThinkSpeak() {
  /*
  int len = sizeof(sensorsDataName) / sizeof(sensorsDataName[0]);
  
  for(int i = 0; i < len; i++) {
    
    
   valSensor = getSensorData(sensorsData[i]);
   String getData = "GET /update?api_key="+ API +"&"+ "field" + i  +"="+String(valSensor);
   sendCommand("AT+CIPMUX=1",5,"OK");
   sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
   sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
   esp8266.println(getData);delay(1500);countTrueCommand++;
   sendCommand("AT+CIPCLOSE=0",5,"OK");
   
  }*/
}


/*
int getSensorData(String dataName){
  Serial.println("Sent data to the server");
  // float t = dht.readTemperature();
  // return t; // Replace with your own sensor code


  if(dataName == sensorsData[0]) {
    float t = dht.readTemperature();
    return t;
  } else if(dataName == sensorsData[1]){
    
  }

}*/

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
