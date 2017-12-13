/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME    "Brynjar"
#define IO_KEY         "b3d5790e424644f1b81781af457b9a76"

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056

#define WIFI_SSID       "iot-research"
#define WIFI_PASS       "yNUtn6uwGa7PFrLdv3hgqDRpf89MsTHb"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


/******************************* SMTP **************************************/
const char smtp_server[ ] = "smtp.gmail.com";      // SMTP server
const char smtp_user_name[ ] = "iotsecsys@gmail.com";   // SMTP user name
const char smtp_password[ ] = "mghcirdrohzzjbwa";    // SMTP password
const char smtp_port[ ] = "465";              // SMTP server port


/******************************* Sender/Receiver ***************************/
const char sender_address[ ] = "iotsecsys@gmail.com";    // Sender address
const char sender_name[ ] = "Home Security System";      // Sender name

const char to_address[ ] = "iotsecsys@gmail.com";        // Recipient address
const char to_name[ ] = "Owner";           // Recipient name

char subject[ ] = "Humidity alert";
const char body[ ] = "Hello. Humidity is over 40%";   //message to be sent as email.

