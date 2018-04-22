// Water Sensor that will text me if the sensor goes off.
// Information available at Ubidot.com
// Created by jnissen using Ubidots library 
// May 16th, 2017

/****************************************
   Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-v57qnu6xCWhJ3rrxhvXVAlPs4SH3se" // Your Ubidots TOKEN
#define WIFINAME "Honor8" //Your SSID
#define WIFIPASS "1234567c" // Your Wifi Password

#define sensor    A0       // Hook water sensor to pin A0 of NODEMCU module
#define LED       D4       // Led in NodeMCU/WeMos D1 Mini at pin GPIO2 (D4)
#define BRIGHT    350      // Max led intensity (1-500)
#define INHALE    2500     // Inhalation time in milliseconds.
#define PULSE     INHALE*1000/BRIGHT
#define REST      10000    // Rest Between Inhalations.
#define RELAY_PIN 14 // RELAY PIN

Ubidots client(TOKEN);

/****************************************
   Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  /*
   * 
   * 
   */
}

/****************************************
   Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(sensor, INPUT);   // Analog pin setup for read
  pinMode(LED, OUTPUT);     // LED pin as output.
  client.setDebug(true);    // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  // Make sure the water sensor is connected to your WiFi network. If not try to reconnect. 
  if (!client.connected()) {
    Serial.print("Not Connected!!");
    client.reconnect();
  }

  if (client.connected()) {
    Serial.print("Connected!!!");
  }

  // A fancy delay routine! Makes the LED appear to "breathe" by slowly ramping up and down. This is to show you the water sensor is working 
  // by slowly making the LED on the board glow from dim to bright and back dim. It's not really needed but it's a bit of fun to watch.
  // If you don't add some delay the water sensor will attempt to "log" data often and your free Ubidots account may be disabled. This 
  // delay is intended to create a new data point once every 15 seconds or so. 
 /* 
  for (int i = 1; i < BRIGHT; i++) {
    digitalWrite(LED, LOW);            // turn the LED on.
    delayMicroseconds(i * 10);         // wait
    digitalWrite(LED, HIGH);           // turn the LED off.
    delayMicroseconds(PULSE - i * 10); // wait
    delay(0);                          // to prevent watchdog firing.
  }
  //ramp decreasing intensity, Exhalation (half time):
  for (int i = BRIGHT - 1; i > 0; i--) {
    digitalWrite(LED, LOW);            // turn the LED on.
    delayMicroseconds(i * 10);         // wait
    digitalWrite(LED, HIGH);           // turn the LED off.
    delayMicroseconds(PULSE - i * 10); // wait
    i--;
    delay(0);                          // to prevent watchdog firing.
  }
  delay(REST);                         // take a rest...
*/
  // Publish values of ADC0 water sensor. Water will cause the voltage to rise and the ADC will read this as a higher value.
  // Once the value is read the NODEMCU will publish it to UBIDOTS. The Node MCU does not care what the reading is. It only reports it.
  // If below trigger value the text message will NOT be delivered. Above trigger it's sent.

  float adcValue = analogRead(sensor);    // Read the ADC channel
  Serial.println("1.");
  Serial.println(adcValue);
  Serial.println();

if( adcValue > 200){
   digitalWrite(LED,LOW);
   //digitalWrite(14,LOW);
   }else{
   digitalWrite(LED,HIGH);
   //digitalWrite(14,HIGH);
   }
  
  client.add("h2o_heater", adcValue);     // Variable for the water heater sensor assigned the ADC value. This will show up in Ubidots within the water-sensor device
  client.ubidotsPublish("water-sensor");  // Device name for Ubidots. Make sure Ubidots is setup prior to loading and running this application. 
  client.loop();
}
