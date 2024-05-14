#define BLYNK_TEMPLATE_ID "TMPL60RfMWo_K"
#define BLYNK_TEMPLATE_NAME "UTP TRIO SAKTI ARDIKA"
#define BLYNK_AUTH_TOKEN "5w5cFN52yunO7GCeEzvHSBu1BWIx1Ms0"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

Ultrasonic ultrasonic(4,16);
int pinled = 14;
int buzzerPin = 25;
double distance = 0;
int switchBuzz = 0;

// Menyalakan buzzer
void myTone(int pin){
  ledcAttachPin(pin, 0); // pin channel
  ledcWriteNote(0, NOTE_F, 8); // chanel, freq, oct
}

// Mematikan buzzer
void myNoTone(int pin){
  ledcDetachPin(pin);
}

//setting switch
BLYNK_WRITE(V2){
  int pin_value = param.asInt();
  Serial.println(pin_value);
  if (pin_value == 1){
    switchBuzz = 1;
  }
  else{
    switchBuzz = 0;
  }
}

void sendSensor(){
  delay(1000);
  double distance = ultrasonic.read(CM);
  Blynk.virtualWrite(V0, distance);
  Serial.print("jarak : ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < 200){

    if(switchBuzz == 1){
    digitalWrite(pinled, HIGH);
    Serial.println("Hama Mendekat dalam radius kurang dari 2 meter");
    Blynk.virtualWrite(V1, 1);
    myTone(buzzerPin);
    Blynk.logEvent("notifikasi", "Ada Hama Mendekat dalam radius kurang dari 2 meter");
    }
    else{
      digitalWrite(pinled, LOW);
      Serial.println("Buzzer dan Notifikasi dimatikan");
      Blynk.virtualWrite(V1, 0);
      myNoTone(buzzerPin);
    }
  }
  else{
      digitalWrite(pinled, LOW);
      Serial.println(" Tidak ada hama dalam radius 2 meter");
      Blynk.virtualWrite(V1, 0);
      myNoTone(buzzerPin);

  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(pinled, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2500L, sendSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // this speeds up the simulation
  Blynk.run();
  sendSensor();
  timer.run();
}