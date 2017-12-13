#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;

int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed

/*void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}*/
void setup() {
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
}

void loop() {
  if (calls <= maxCalls) {
    Serial.println("Running SendEmail - Run #" + String(calls++));

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
    String SubjectValue = "Netprufa";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String ToAddressValue = "iotsecsys@gmail.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);
    String PasswordValue = "pkxdnenrtsofdjar";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String MessageBodyValue = "Netprufa";
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
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between SendEmail calls
}
