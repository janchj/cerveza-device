// This #include statement was automatically added by the Spark IDE.
#include "DS18B20/Particle-OneWire.h"
#include "DS18B20/DS18B20.h"
#include <HttpClient.h>

//DS18B20 ds18b20 = DS18B20(D2); //Sets Pin D2 for Water Temp Sensor
HttpClient http;

// set the http headers
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

// init variables
http_request_t request;
http_response_t response;

// temperature variables
DS18B20 ds18b20 = DS18B20(D2); //Sets Pin D2 for Water Temp Sensor
int led = D7;
char szInfo[64];
float pubTemp;
double celsius;
double fahrenheit;
unsigned int Metric_Publish_Rate = 60000;
unsigned int MetricnextPublishTime = 60000;
int DS18B20nextSampleTime = 60000;
int DS18B20_SAMPLE_INTERVAL = 60000;
int dsAttempts = 0;

void setup() {
    //Time.zone(-5);
    //Particle.syncTime();
    pinMode(D2, INPUT);
    Serial.begin(115200);
}

// read refresh interval
// read temp
// push to remote


void loop() {
    // read temperature from sensor
    getTemp();

    // publish to remote
    publishData();

    // refresh after
    delay(60000);
}

void publishData(){
  request.hostname = "stark-lowlands-38120.herokuapp.com";
  request.port = 80;
  request.path = "/devices/3e0027000547343233323032/log";
  request.body = "{\"temp\":" + String(celsius) + "}";

  // submit temp log
  //Particle.publish("temp", celsius);
  http.post(request, response, headers);
}

void getTemp(){
    if(!ds18b20.search()){
      ds18b20.resetsearch();
      celsius = ds18b20.getTemperature();
      //Serial.println(celsius);
      while (!ds18b20.crcCheck() && dsAttempts < 4){
        Serial.println("Caught bad value.");
        dsAttempts++;
        Serial.print("Attempts to Read: ");
        Serial.println(dsAttempts);
        if (dsAttempts == 3){
          delay(1000);
        }
        ds18b20.resetsearch();
        celsius = ds18b20.getTemperature();
        continue;
      }
      dsAttempts = 0;
      //fahrenheit = ds18b20.convertToFahrenheit(celsius);
      //DS18B20nextSampleTime = millis() + DS18B20_SAMPLE_INTERVAL;
      //Serial.println(fahrenheit);
    }
}
