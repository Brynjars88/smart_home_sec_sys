// Smart home security system with following sensors:
// - Temperature
// - Humidity
// - Motion
// - CO (for smoke detection)
// The system is connected to a camera which snaps a picture if any of those
// sensors detect an anomaly (max values should be defined in config) and sends
// to an email address from config.

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>

#include "config.h"


WiFiClient client;

Adafruit_SSD1306 oled = Adafruit_SSD1306();  // oled display
#define DATA_PIN 2  // pin connected to DHT22 data line
DHT_Unified dht(DATA_PIN, DHT22);  // create DHT22 instance


/****** Motion sensor ******/
int ledPin = 15;                // choose the pin for the LED
int inputPin = 12;              // choose the input pin (for PIR sensor)
int signalPin = 0;              // Set to HIGH to take a picture
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
bool email_sent = false;


void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(signalPin, OUTPUT);   // declare signalpin as output
  
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  oled.display();
  
  // text display tests
  oled.setTextSize(1);
  oled.setTextColor(WHITE);

  Serial.begin(9600);
  delay(4000);  // buy time for a human to open the serial window

   // print it to the OLED
  oled.clearDisplay();
  oled.setCursor(0, 0);

  oled.display();
  delay(2000);

  Serial.println("start of temboo test");
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Waiting For WiFi connection: ");
  oled.display();

  int counter = 0;
  // Try to connect to the local WiFi network
  WiFi.begin(WIFI_SSID, WPA_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { // wait for the loging process to complete
    delay(1500);
    Serial.print(".");
    oled.print(".");
    oled.display();
    counter = counter + 1;
      if(counter > 3)
      {
         oled.clearDisplay();
         oled.setCursor(0, 0);
         oled.print("Waiting For WiFi connection: ");
         counter = 0;
      }
  }
  Serial.println("");
  Serial.print("WiFi connected - IP address of ESP8266 is:");
  Serial.println(WiFi.localIP());
  oled.clearDisplay();
  oled.print("Connected To IoT-research !");
  oled.display();

  delay(3000);

  // initialize dht22
  dht.begin();
}


void loop() {

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  dht.humidity().getEvent(&event);
  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println("%");

  // print it to the OLED
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("SSID: "); oled.println(WIFI_SSID);
  oled.print("IP: "); oled.println(WiFi.localIP());
  oled.print("Temp: "); oled.print(celsius, 0); oled.print(" °C ");
  oled.print("Hum: "); oled.print(event.relative_humidity, 0); oled.println(" %");

  oled.display();
  delay(2000);


  /****** Humidity anomaly ******/
  if (event.relative_humidity >= maxHumidity) {
    String SubjectValue = "Humidity is:" + String(event.relative_humidity);
    String MessageBodyValue = "Humidity is:" + String(event.relative_humidity);
    Serial.println("Detected high humidity - Running SendEmail");
    send_email(MessageBodyValue, SubjectValue);
  }

  /****** Temperature anomaly ******/
  if (celsius >= maxTemperature) {
    String SubjectValue = "Temperature is:" + String(celsius);
    String MessageBodyValue = "Temperature is:" + String(celsius);
    Serial.println("Detected high temperature - Running SendEmail");
    send_email(MessageBodyValue, SubjectValue);
  }

  /****** Motion detection ******/
  val = digitalRead(inputPin);    // read input value
  if (val == HIGH) {              // check if the input is HIGH
    digitalWrite(ledPin, HIGH);   // turn LED ON
    digitalWrite(signalPin, HIGH); // send signal to take a picture
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;

      if (not email_sent) {
        email_sent = not email_sent;
        String SubjectValue = "Motion detected, call the police!";
        String MessageBodyValue = "Motion detected, call the police!";
        Serial.println("Detected high humidity - Running SendEmail");
        send_email(MessageBodyValue, SubjectValue);
      }
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    digitalWrite(signalPin, LOW); // we dont want to take a picture...
    if (pirState == HIGH) {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}


void send_email(String MessageBodyValue, String SubjectValue) {

  TembooChoreo SendEmailChoreo(client);

  // Invoke the Temboo client
  SendEmailChoreo.begin();

  // Set Temboo account credentials
  SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs
  String FromAddressValue = "iotsecsys@gmail.com";
  String UsernameValue = "iotsecsys@gmail.com";
  String ToAddressValue = "iotsecsys@gmail.com";
  String PasswordValue = "pkxdnenrtsofdjar";
  SendEmailChoreo.addInput("FromAddress", FromAddressValue);
  SendEmailChoreo.addInput("Username", UsernameValue);
  SendEmailChoreo.addInput("Subject", SubjectValue);
  SendEmailChoreo.addInput("ToAddress", ToAddressValue);
  SendEmailChoreo.addInput("Password", PasswordValue);
  SendEmailChoreo.addInput("MessageBody", MessageBodyValue);

  // Identify the Choreo to run
  SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

  // Run the Choreo; when results are available, print them to serial
  SendEmailChoreo.run();

  while (SendEmailChoreo.available()) {
    char c = SendEmailChoreo.read();
    Serial.print(c);
  }
  SendEmailChoreo.close();
  delay(30000);
}

