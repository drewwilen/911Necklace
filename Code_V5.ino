#include "Adafruit_FONA.h"
//#include "cactus_io_DHT22.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4 //sms

//#define DHT22_PIN 5 //temp

//HT22 dht(DHT22_PIN);

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

#define phoneNumber "19147037896"



void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("SMS Messages Sender");

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));



  //dht.begin();




}

void loop() {
  //String text = temp();
  String location = gps();
  String messages = location;
  char txt[messages.length() + 1];
  messages.toCharArray(txt, messages.length());

  Serial.println(txt);
  flushSerial();
    Serial.println(phoneNumber);
    Serial.println(txt);
    if (!fona.sendSMS(phoneNumber, txt)) {
    Serial.println(F("Failed"));
    } else {
    Serial.println(F("Sent!"));
    }

    Serial.println("\nCOMPLETE!\n");
    delay(3600000);

}

void flushSerial() {
  while (Serial.available())
    Serial.read();
}



String gps() {


  String lat, lon;
  Serial.println(F("Enabling GPS..."));
  fona.enableGPS(true);

  delay(2000);
  float latitude, longitude;
  while (!fona.getGPS(&latitude, &longitude));
  // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
  lat = String(latitude, 6);
  lon = String(longitude, 6);
  return "Emergency Message would be here... \n My location: www.google.com/maps/search/" + lat + "," + lon;
}
