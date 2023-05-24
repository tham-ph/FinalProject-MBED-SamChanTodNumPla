/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID   "TMPL6combUjXV"
#define BLYNK_TEMPLATE_NAME "BinProj"
#define BLYNK_AUTH_TOKEN    "D9pniHRkJ79Mx6svLQm1LgCBR7YLRgrq"

#include <ESP8266WiFi.h>  
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

/*Change wifi's name and password*/
char ssid[] = "Sat";
char pass[] = "12345678";

/*for ultrasonic sensor*/
#define Ultrasonic_PIN D6
#define BLYNK_LED_PIN V1
bool LED = LOW;

void readUltrasonic(){

  if (digitalRead(Ultrasonic_PIN) != LED) Serial.println("CLED");
  LED = digitalRead(Ultrasonic_PIN);
  Blynk.virtualWrite(BLYNK_LED_PIN, LED);

}

/* for load cell*/
#define BLYNK_WEIGHT_PIN V0
SoftwareSerial WeightSerial(D7, D8);  //Rx,TX
String weight = "";

void readLoadCell(){
  weight = "";
  while (WeightSerial.available() > 0)
  {
    weight += (char)WeightSerial.read();
  }
  Serial.print(weight);  
  if (weight != "") Blynk.virtualWrite(BLYNK_WEIGHT_PIN, max(0,(int)weight.toInt()-20));
}

/*set up*/
void setup(){
  Serial.begin(9600);                           //for serial monotir
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);    //for connect Blynk
  WeightSerial.begin(115200);                   //for serial from loadcell
  pinMode(Ultrasonic_PIN, INPUT);               //for digital input from ultrasonic 
}

/*loop*/
void loop(){

  Blynk.run();        //connect blynk
  readUltrasonic();   //recieve ultrasonic
  readLoadCell();     //recieve loadcell

}