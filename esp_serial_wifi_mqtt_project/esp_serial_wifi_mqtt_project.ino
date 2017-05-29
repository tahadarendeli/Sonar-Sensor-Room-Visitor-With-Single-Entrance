#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "EE321StudyRoom"
#define WLAN_PASS       "studyRoom"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Your_UserName"
#define AIO_KEY         "Your_AIO_Key"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'studyRoom' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish studyRoomIn = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/study-room-in-test");
Adafruit_MQTT_Publish studyRoomOut = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/study-room-out-test");
Adafruit_MQTT_Publish studyRoomVis = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/study-room-test");

//data variables
int peopleIn = 0;
int peopleOut = 0;
int visitors = 0;

void setup() {
  Serial.begin (115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
}

void loop() {
  MQTT_connect();
  int availableData = Serial.available();
  if (availableData > 0) {
    int dataArr[availableData];
    for (int i = 0; i < availableData; i++) {
      dataArr[i] = Serial.read();
    }
    peopleIn = (int) dataArr[0];
    peopleOut = (int) dataArr[2];
    visitors = peopleIn - peopleOut;
    // Now we can publish stuff!
    if (visitors > 0) {
      studyRoomIn.publish(peopleIn);
      studyRoomOut.publish(peopleOut);
      studyRoomVis.publish(visitors);
    }
  }
}
