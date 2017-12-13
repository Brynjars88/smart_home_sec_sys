// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "temboo_wifi_config.h"

/************************ Example Starts Here *******************************/
#include <Temboo.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


WiFiClient client;

// oled display
Adafruit_SSD1306 oled = Adafruit_SSD1306();

// pin connected to DH22 data line
#define DATA_PIN 2

// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT22);

/* set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");*/

int ledPin = 15;                // choose the pin for the LED
int inputPin = 12;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
bool email_sent = false;

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  
  Serial.begin(9600);
  delay(4000);  // buy time for a human to open the serial window

  Serial.println("start of temboo test");

  // Try to connect to the local WiFi network
  WiFi.begin(WIFI_SSID, WPA_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { // wait for the loging process to complete
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - IP address of ESP8266 is:");
  Serial.println(WiFi.localIP());
  delay(3000);
  
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  oled.display();

  // initialize dht22
  dht.begin();

  // text display tests
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
}

void loop() {

  /* io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();*/

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  // float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  // Serial.print("fahrenheit: ");
  // Serial.print(fahrenheit);
  // Serial.println("F");

  /* save fahrenheit (or celsius) to Adafruit IO
  temperature->save(celsius);*/

  dht.humidity().getEvent(&event);
   
  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  /* save humidity to Adafruit IO
  humidity->save(event.relative_humidity);*/

  // print it to the OLED
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.print("SSID: "); oled.println(WIFI_SSID);
  oled.print("IP: "); oled.println(WiFi.localIP());
  oled.print("Temp: "); oled.print(celsius,0); oled.print(" °C ");
  oled.print("Hum: "); oled.print(event.relative_humidity,0); oled.println(" %");
  
  oled.display();

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(2000);
  if(event.relative_humidity > 40)
  {
    Serial.println("Detected high humidity - Running SendEmail");

    TembooChoreo SendEmailChoreo(client);

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String FromAddressValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("FromAddress", FromAddressValue);
    String UsernameValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("Username", UsernameValue);
    String SubjectValue = "Humidity is:" + String(event.relative_humidity);
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String ToAddressValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    String PasswordValue = "pkxdnenrtsofdjar";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String MessageBodyValue = "Humidity is:" + String(event.relative_humidity);
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
    
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();

    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();
    delay(30000);
    
  }

  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;

    if(email_sent == false)
    {
      email_sent == true;
    Serial.println("Detected motion - Running SendEmail");

    TembooChoreo SendEmailChoreo(client);

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String FromAddressValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("FromAddress", FromAddressValue);
    String UsernameValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("Username", UsernameValue);
    String SubjectValue = "Motion detected, call the police!";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String ToAddressValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    String PasswordValue = "pkxdnenrtsofdjar";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String MessageBodyValue = "Motion detected, call the police!";
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
    
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();

    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
      SendEmailChoreo.close();
      delay(30000);
      }
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
