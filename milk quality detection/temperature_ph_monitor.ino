#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int potPin=A0;
float ph,temp;
float Value=0;
 

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


String apiKey = "EWNE19AHJIUX55RW"; // Enter your Write API key from ThingSpeak
const char *ssid = "OPPOA5s"; // replace with your wifi ssid and wpa2 key
const char *password = "12345678";
const char* server = "api.thingspeak.com";// don't change this
 
WiFiClient client;
 
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(potPin,INPUT);
  delay(1000);

  sensors.begin();   //for temprature sensor
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}
 
void loop() {
  // put your main code here, to run repeatedly:
Value= analogRead(potPin);
// Serial.print(Value);
// Serial.print(" | ");
float voltage=Value*(3.3/4095.0);
ph=(3.3*voltage);

// if(temp < ph && temp < 7.0){
//   temp = ph;
//   Serial.println(ph);
// }

 sensors.requestTemperatures(); 

  // float temperatureC = sensors.getTempCByIndex(0);
  // Serial.print(temperatureC);
  // Serial.println("ºC");

  float temperatureF = sensors.getTempFByIndex(0);

  Serial.print(temperatureF);
  Serial.println("ºF");

  Serial.println(ph);
  delay(500);
 
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
   {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(ph);
      postStr += "\r\n";
    
     String postStr2 = apiKey;
      postStr2 += "&field2=";
      postStr2 += String(temperatureF);
      postStr2 += "\r\n";


      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print(postStr2.length());
      client.print("\n\n");
      client.print(postStr); 
      client.print(postStr2); 
    }    
}
