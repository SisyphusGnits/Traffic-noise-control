#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#define SoundSensorPin A0
#define VREF 3.3

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//wifi params
const char* ssid = "GNITS_MARGAH";
const char* password = "margah19";

// CSE params
//const char* host = "192.168.227.9";
//192.168.133.9
const char* host = "192.168.227.9";
const int httpPort = 8453;
String url = "/~/in-cse/in-name/AE-Sisphyus/Node-1/Data/";
int ty = 4;
const char* origin   = "admin:admin";


const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
int nextState;                // The next state the machine will enter
int currentState;             // The current state the machine is in
float voltageValue,dbValue;
signed long startTime;      // Holds a record of the loop start time
signed long elapsedTime;    // Accumulates the loop time
signed long red_timer = 20000;
signed long countdown=10;
signed long green_timer = 30000;
int count = 0;
float threshold = 80;


String node_id = "data_";
int timeout = red_timer;
String light_status = "GREEN";
int timeout_increased = 0;
float dbvalue = 0.0;

//
// States the machine can be in
//
#define STATE_GREEN_INIT 1
#define STATE_GREEN 2
#define STATE_FLASHING_AMBER_ON 3
#define STATE_FLASHING_AMBER_OFF 4
#define STATE_FLASHING_AMBER_TIMEOUT 5
#define STATE_RED 12
#define SoundSensorPin A0
#define VREF 3.2

int numAmberFlashes;        // The number of times the amber light flashes
int punishments = 0;

void setup() {
  nextState = STATE_GREEN;
  numAmberFlashes = 0;
  pinMode(2, OUTPUT); //green
  pinMode(0, OUTPUT); //yellow
  pinMode(16, OUTPUT); //red
    pinMode(13, OUTPUT); //green
  pinMode(12, OUTPUT); //yellow
  pinMode(14, OUTPUT); //red
  Serial.begin(115200);

  WiFi.begin(ssid, password);
    while( WiFi.status() != WL_CONNECTED) 
    {
        Serial.print("#");
        delay(300);
        // don't do anything else
    }
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);              //OLED display start
  // display.display();                                     //show buffer
   display.setTextSize(1);                                //Set text size to 1 (1-6)
   display.setTextColor(WHITE);                           //Set text color to WHITE (no choice lol)
   display.setCursor(0,0);                                //cursor to upper left corner
   //delay(2000);    
}

void loop() {
  // record the time
  startTime = millis();
  static int i=1;
  node_id = "data_";

    voltageValue=analogRead(SoundSensorPin) / 1024.0 * VREF;
    dbValue = voltageValue * 50.0;
    dbvalue = dbValue; //variable to send to om2m
    Serial.print(dbValue,1);
    Serial.println("   ");
    delay(1000);
     currentState = nextState;
  // Decode and execute the current state
  switch(currentState) {
    case STATE_GREEN:
      light_status = "GREEN"; //variable to send to om2m
      displayGREENOLED();
      if( elapsedTime >= green_timer ) { // 3 second timeout
        elapsedTime = 0;
        nextState = STATE_FLASHING_AMBER_ON;
      }

      showGreen();
      showRed1();


    break;
    case STATE_FLASHING_AMBER_ON:
      light_status = "AMBER"; //variable to send to om2m
      if( elapsedTime >= 400 ) { // 1 second timeout
        elapsedTime = 0;
       nextState = STATE_FLASHING_AMBER_OFF;
        
              flashAmberOn();
              flashAmberOn1();

      }
    break;
    case STATE_FLASHING_AMBER_OFF:
      if( elapsedTime >= 400 ) { // 1 second timeout
        elapsedTime = 0;
        numAmberFlashes++;
        if( numAmberFlashes < 2 ) {
          nextState = STATE_FLASHING_AMBER_ON;
        } else {
          numAmberFlashes = 0;
          nextState = STATE_RED;          
        }
      }
            flashAmberOff();
            flashAmberOff1();

    break;
    case STATE_RED:
      light_status = "RED";
      displayREDOLED();
      
      if (dbValue > 80) {
        count = count + 1;
        Serial.println("-----------------");
        Serial.print("Crime ");
        Serial.println(count);
        if (count > 3) {
        if (red_timer < 50000){        
        red_timer = red_timer + 11000;
        timeout = red_timer; //variable to send to om2m
        timeout_increased = 1; //variable to send to om2m
        count = 0;
        punishments = punishments + 1;
        }
        else{
        red_timer = red_timer;
        timeout_increased = 0; //variable to send to om2m
        }
      }

      }

      if( elapsedTime >= red_timer) { // 5 second timeout
        elapsedTime = 0;
        nextState = STATE_GREEN;
        count = 0;
        red_timer = 20000;
      }
      
        showRed();
        showGreen1();

    break;
  }
 node_id = node_id + String(i++);
 String con = "[\'"+String(node_id)+"\'" + "," +  dbvalue + "," + "\'" + light_status + "\'" + "," + timeout + "," + timeout_increased + "]";
 //"cin_"+ String() + String(i++)
  
 String data = "{\"m2m:cin\": {"

  + String()  + "\"con\": \"" + con + "\","

  + String()  + "\"rn\": \"" + node_id + "\","

  + String()  + "\"cnf\": \"text\""

  + String()  + "}}";

  WiFiClient client;
  delay(3000);
  
   if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");  
  }
  else{
    String req = String()+"POST " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "X-M2M-Origin: " + origin + "\r\n" +
               "Content-Type: application/json;ty="+ty+"\r\n" +
               "Content-Length: "+ data.length()+"\r\n"
               "Connection: close\r\n\n" + 
               data;
  Serial.println(req+"\n");

  // Send the HTTP request
  client.print(req);
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
     }
  }
    
    }
  // switch to the next state
  elapsedTime += millis() - startTime;

  // Accumulate elapsed time
}

void showGreen() {
  Serial.println("GREEN LIGHT ON");
  digitalWrite(2, HIGH);
  digitalWrite(0, LOW);
  digitalWrite(16, LOW);
}

void displayGREENOLED(){

  display.setCursor(0,0);                                //cursor to upper left
  display.setTextSize(5);                                //set text size to 2
  display.print(((green_timer + 1000 - elapsedTime)/1000));           //write calibrated deciBels
  display.display();
  display.clearDisplay();

}

void displayREDOLED(){
  
  display.setCursor(0,0);                                //cursor to upper left
  display.setTextSize(5);                                //set text size to 2
  display.print(((red_timer + 1000 - elapsedTime)/1000));                                //write calibrated deciBels
  display.display();
  display.clearDisplay();
}

void flashAmberOn() {

  digitalWrite(2, LOW);
  digitalWrite(0, HIGH);
  digitalWrite(16, LOW);  
  Serial.println("Yellow ON LIGHT");
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print("YOU ARE ATPUNISHMENT  SIGNAL");
    display.display();
  display.clearDisplay();
}

void flashAmberOff() {
  digitalWrite(2, LOW);
  digitalWrite(0, LOW);
  digitalWrite(16, LOW);  
  Serial.println("Yellow OFF LIGHT");
  display.setCursor(0,0);
  display.setTextSize(2.5);
  display.print("NO HONKING    ON    RED SIGNAL");
    display.display();
  display.clearDisplay();
}

void showRed() {
  Serial.println("RED LIGHT ON");
  digitalWrite(2, LOW);
  digitalWrite(0, LOW);
  digitalWrite(16, HIGH);  
}


void showGreen1() {
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
}


void flashAmberOn1() {

  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(14, LOW);  

}

void flashAmberOff1() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);  
}

void showRed1() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, HIGH);  
}

/*
#include <ESP8266WiFi.h>
#define SoundSensorPin A0
#define VREF 3.3

// WIFI params
const char* ssid = "HUAWEI0001707";
const char* password = "102lxmndm";
String node_id = "cin_";
int timeout = 3000;
String light_status = "GREEN";
int timeout_increased = 0;
float dbvalue = 67.8;

// CSE params
//const char* host = "192.168.226.9";
//192.168.133.9
const char* host = "192.168.1.43";
const int httpPort = 8453;
String url = "/~/in-cse/in-name/AE-Sisphyus/Node-1/Data/";
int ty = 4;
const char* origin   = "admin:admin";

void setup() {
   Serial.begin(115200);
    WiFi.begin(ssid, password);
    while( WiFi.status() != WL_CONNECTED) 
    {
        Serial.print("#");
        delay(300);
        // don't do anything else:
    }
}

void loop() {
 node_id = node_id + String(i++);
 String con = "[\'"+String(node_id)+"\'" + "," +  dbvalue + "," + "\'" + light_status + "\'" + "," + timeout + "," + timeout_increased + "]";

 //"cin_"+ String() + String(i++)
  
 String data = "{\"m2m:cin\": {"

  + String()  + "\"con\": \"" + con + "\","

  + String()  + "\"rn\": \"" + node_id + "\","

  + String()  + "\"cnf\": \"text\""

  + String()  + "}}";

  WiFiClient client;
  delay(3000);
  
   if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    
    
  }
  else{
    String req = String()+"POST " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "X-M2M-Origin: " + origin + "\r\n" +
               "Content-Type: application/json;ty="+ty+"\r\n" +
               "Content-Length: "+ data.length()+"\r\n"
               "Connection: close\r\n\n" + 
               data;

  Serial.println(req+"\n");

  // Send the HTTP request
  client.print(req);
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
     }

  }
    
    
    }
    

}
      
*/
