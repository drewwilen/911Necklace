/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:
// Include the GSM library
// Beware of the texting, and make sure we don't loop it in the text messagins (SMS)
#include "Adafruit_FONA.h"
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4 //sms


#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

#define phoneNumber "19147037896"


const int buttonPin = 7;     // the number of the pushbutton pin
const int buttonPin2 = 11;     // the number of the pushbutton pin
const int ledPin =  9;      // the number of the LED pin
boolean sendmessage = false;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status

//String location;
//String messages;
//char txt;
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
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


}

void loop() {

  //Serial.println("SENDING!!");
  String location = gps();
  String messages = location;
  char txt[messages.length() + 1];
  messages.toCharArray(txt, messages.length());
  Serial.println(phoneNumber);
  Serial.println(txt);

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.println(txt);
  if (buttonState == HIGH) {
    flushSerial();
    if (!fona.sendSMS(phoneNumber, txt)) {
      Serial.println(F("Failed"));
      digitalWrite(ledPin, HIGH);

    } else {
      Serial.println(F("Sent!"));
      digitalWrite(ledPin, HIGH);
    }
    //digitalWrite(ledPin, HIGH);

    Serial.println("\nCOMPLETE!\n");


    delay(3600000);

  } else if (buttonState == LOW) {
    //sendmessage = f;
    Serial.println("NOT SENDING!!");
    //sendmessage = false;
    digitalWrite(ledPin, LOW);
  }

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  //  if (sendmessage == true) {
  // turn LED on:
  //    digitalWrite(ledPin, HIGH);
  //        Serial.println("SENDING!!");
  //
  //    String location = gps();
  //  String messages = location;
  //  char txt[messages.length() + 1];
  //  messages.toCharArray(txt, messages.length());
  //
  //  Serial.println(txt);
  //  flushSerial();
  //    Serial.println(phoneNumber);
  //    Serial.println(txt);
  //    if (!fona.sendSMS(phoneNumber, txt)) {
  //    Serial.println(F("Failed"));
  //    } else {
  //    Serial.println(F("Sent!"));
  //    }
  //
  //    Serial.println("\nCOMPLETE!\n");
  //    delay(3600000);
  //}else {
  //  // turn LED off:
  //  digitalWrite(ledPin, LOW);
  //  Serial.println("Not Sending");
  //
  //}

  char number[30] = "19147037896";
  flushSerial();
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    Serial.print(F("Calling ")); Serial.println(number);
    if (!fona.callPhone(number)) {
      Serial.println(F("Failed"));
       digitalWrite(ledPin, LOW);

    } else {
      Serial.println(F("Sent!"));
      digitalWrite(ledPin, HIGH);
        //delay(2000);

      while (true) {
        buttonState2 = digitalRead(buttonPin);
        if (buttonState2 == HIGH) {

          if (! fona.hangUp()) {
            Serial.println(F("Failed"));
          } else {
            Serial.println(F("OK!"));
          }
            break;
          
        }
      }
    }
  }
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
  //return "HELP! ITS AN EMERGENCY!! COME QUICK \n My location: www.google.com/maps/search/" + lat + "," + lon;
  return "My location: www.google.com/maps/search/" + lat + "," + lon;

  //https://www.google.com/maps/search/b;ababdjfnajs/@41.035332,-73.8066048,15z

}
