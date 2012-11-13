/*
Developed by the LAB at Rockwell Group

See "basic protoshield setup.PNG" in the examples directory
to see how this example is expecting the Arduino to be wired up.
I have the Ethernet shield sitting on top of the arduino, and then the
Sparkfun ProtoShield on top of that.

You must remember to include the Ethernet library, SPI library, 
and the WebSocketClient library 
(accessible here: https://github.com/labatrockwell/ArduinoWebsocketClient)
in order to use the Spacebrew library.

visit http://docs.spacebrew.cc/ for more info about Spacebrew!

putting the "toty" back in "Prototyping"
*/


#include <SPI.h>
#include <Spacebrew.h>
#include <Ethernet.h>
#include <WebSocketClient.h>

uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
Spacebrew spacebrewConnection;
int lastButton = LOW;

const int buttonPin = 7;
const int ledPin = 8;
const int spacebrew_ledPin = 9;

void setup() {
  //connect to message callbacks
  spacebrewConnection.onBooleanMessage(onBooleanMessage);
  
  //register publishers and subscribers
  spacebrewConnection.addPublish("Button", SB_BOOLEAN);
  spacebrewConnection.addSubscribe("Blink LED", SB_BOOLEAN);
  
  //connect to the spacebrew server
  Ethernet.begin(mac);
  spacebrewConnection.connect("lab-macbookpro-02.rockwellgroup.com", "Rename_Me", "Arduino Input and Output Test");
  
  pinMode(ledPin, OUTPUT);
  pinMode(spacebrew_ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  //let the spacebrew library check for any incoming messages
  //and re-connect to the server if necessary
  spacebrewConnection.monitor();
  
  int buttonIn = digitalRead(buttonPin);
  if (buttonIn != lastButton){
    boolean buttonPressed = (buttonIn == LOW);
    int ledValue;
    if (buttonPressed){
      ledValue = HIGH;
    } else {
      ledValue = LOW;
    }
    digitalWrite(ledPin, ledValue);
    //send the button state via spacebrew
    spacebrewConnection.send("Button", buttonPressed);
    lastButton = buttonIn;
  }
}

void onBooleanMessage(char *name, bool value){
  //turn the 'digital' LED on and off based on the incoming boolean
  int spacebrew_ledValue;
  if (value){
    spacebrew_ledValue = HIGH;
  } else {
    spacebrew_ledValue = LOW;
  }
  digitalWrite(spacebrew_ledPin, spacebrew_ledValue);
}
