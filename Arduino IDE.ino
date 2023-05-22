#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <Firebase.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

Servo servo1;
Servo servo2; // create servo object to control a servo
int pos = 0;    // variable to store the servo position
#define servoPin1 12
#define servoPin2 14
#define buttonPin1 23
#define buttonPin2 15

const char* ssid     = "Gurrapu-2.4";
const char* password = "Sukram#2";
#define FIREBASE_HOST "https://esp32andfirebase-491cb-default-rtdb.firebaseio.com/IRSensors"  //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "7Gj76gMYUd8iuNYWPM5e0PjDDiPRhhr8sLdF8AQe" 

#define NTP_OFFSET  19800 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.asia.pool.ntp.org"

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

#define sda 21
#define scl 22
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino resetpin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

FirebaseData firebaseData;
FirebaseJson json;

#define IRPin1 32   //32 (We Used GPIO Pin Number)
#define IRPin2 33
#define IRPin3 35
#define relayPin 18

void setup()
{
  display.begin();
  Serial.begin(115200);            // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(IRPin1, INPUT);
  pinMode(IRPin2, INPUT);
  pinMode(IRPin3, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  timeClient.begin();

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData,1000*60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Serial.println("---------------------------------");
  Serial.println("Connected...");
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
    }
    // Clear the buffer.
    display.clearDisplay();
    // Display Text
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 28);
    display.println("Hello Major");
    display.display();
    delay(1000);

}

void loop()
{
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(formattedTime);
  display.display();   // write the buffer to the display
  delay(10);
  delay(100);

  //IR1
  int irvalue1=digitalRead(32);
  Serial.println("IR 1: ");
  Serial.println(irvalue1);
  delay(100);
  json.set("/data1", irvalue1);
  if(irvalue1==1)
  {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Slot1:");
    display.setCursor(35, 20);
    display.println("FREE");
    display.display();   // write the buffer to the display
    delay(500);
  }
  else 
  {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Slot1:");
    display.setCursor(35, 20);
    display.println("NOT FREE");
    display.display();   // write the buffer to the display
    delay(500);
  }
  
  //IR2
  int irvalue2=digitalRead(33);
  Serial.println("IR 2: ");
  Serial.println(irvalue2);
  delay(100);
  json.set("/data2", irvalue2);
  Firebase.updateNode(firebaseData,"/IRSensors",json);
  if(irvalue2==1)
  {
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.println("Slot2:");
    display.setCursor(35,35);
    display.println("FREE");
    display.display();   // write the buffer to the display
    delay(500);
  }
  else 
  {
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.println("Slot2:");
    display.setCursor(35, 35);
    display.println("NOT FREE");
    display.display();   // write the buffer to the display
    delay(500);
   }

   //IR3
  int irvalue3=digitalRead(35);
  Serial.println("IR 3: ");
  Serial.println(irvalue3);
  delay(100);
  json.set("/data3", irvalue3);
  Firebase.updateNode(firebaseData,"/IRSensors",json);
  if(irvalue3==1)
  {
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.println("Slot3:");
    display.setCursor(35,50);
    display.println("FREE");
    display.display();   // write the buffer to the display
    delay(500);
  }
  else 
  {
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.println("Slot3:");
    display.setCursor(35, 50);
    display.println("NOT FREE");
    display.display();   // write the buffer to the display
    delay(500);
   }
  if(irvalue1==0 && irvalue2==0 && irvalue3==0)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.println("ALL SLOTS ARE FILLED.....!");
    display.display();   // write the buffer to the display
    delay(500);
    digitalWrite(18, LOW);
    Serial.println("light is in OFF state");
  }
  else if(irvalue1==1 && irvalue2==1 && irvalue3==1)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.println("ALL SLOTS ARE FREE.....!");
    display.display();   // write the buffer to the display
    delay(500);
    digitalWrite(18., HIGH);
    Serial.println("light is in ON state");
    Serial.println("34 pin ");
  }

  
  int buttonState1 = digitalRead(buttonPin1);
  Serial.println("button 1:");
  Serial.println(buttonState1);
  int buttonState2 = digitalRead(buttonPin2);
  Serial.println("button 2:");
  Serial.println(buttonState2);
  Serial.println("pos: ");
  Serial.println(pos);
  if (buttonState1 == 1) {
     // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(90);              
    delay(2000);                      // waits 20ms for the servo to reach the position
    Serial.println("Gate 1 open");
  } else {
    // sweep from 180 degrees to 0 degrees
    servo1.write(0);
    delay(2000);
    Serial.println("Gate 1 close");
  }
  if (buttonState2 == 1) {
    // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo2.write(90);              
    delay(200);                       // waits 20ms for the servo to reach the position
    Serial.println("Gate 2 open");
  } else {
     
    servo2.write(0);
    delay(200);
    Serial.println("Gate 2 close");
        
  }
}
