
#define BLYNK_TEMPLATE_ID "TMPLxRGNzSRe"
#define BLYNK_DEVICE_NAME "AIR PURIFIER"
#define BLYNK_AUTH_TOKEN "QWLowM5vfuVj6hcHTVi45AJIPxYL9X_5"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <Servo_ESP32.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);
#define in1 2
#define in2 0
#define pwm 16

float measurePin = 33;
int ledPower = 5;
float measurePin1 = 32;
int ledPower1 = 4;
int fdata = 0;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

Servo_ESP32 servo1;

char auth[] = "QWLowM5vfuVj6hcHTVi45AJIPxYL9X_5"; //Enter your Blynk application auth token
char ssid[] = "Xiaomi_12C2"; //Enter your WIFI name
char pass[] = "PasswordOo7"; //Enter your WIFI passowrd


BLYNK_WRITE(V2) {
 fdata = param.asInt();
 servo1.write(fdata);
  Serial.println(fdata); 
}

 void dcfanspeed(){
  
  if(fdata==1){
     ledcWrite(pwmChannel, 40);
     Serial.println("pwm50"); 
     
    lcd.setCursor(12,0);
     lcd.print("FAN");
     lcd.setCursor(12,1);
     lcd.print("SPEED ");
     lcd.setCursor(12,2);
     lcd.print(" 1");
     
  
    }
    else if(fdata==2){ 
    ledcWrite(pwmChannel, 60);
    Serial.println("pwm40");
    
    lcd.setCursor(12,0);
     lcd.print("FAN");
     lcd.setCursor(12,1);
     lcd.print("SPEED ");
     lcd.setCursor(12,2);
     lcd.print(" 2");
    }
     else if(fdata==3){
     ledcWrite(pwmChannel, 80);
     Serial.println("pwm200");
      lcd.setCursor(12,0);
     lcd.print("FAN");
     lcd.setCursor(12,1);
     lcd.print("SPEED ");
     lcd.setCursor(12,2);
     lcd.print(" 3");
    
     }
        else if(fdata==4){   
     ledcWrite(pwmChannel,255);
     Serial.println("pwm255");
    lcd.setCursor(12,0);
     lcd.print("FAN");
     lcd.setCursor(12,1);
     lcd.print("SPEED ");
     lcd.setCursor(12,2);
     lcd.print(" 4");
    
   
      
     }
     else {
     ledcWrite(pwmChannel,0);
     Serial.println("pwm70"); 
      
     }
  
  }

void setup() {
  // put your setup code here, to run once:
ledcSetup(pwmChannel, freq, resolution);
ledcAttachPin(pwm, pwmChannel);
lcd.begin();
  lcd.print("CMREC");
  lcd.setCursor(0,1);
  lcd.print("MECH-C BATCH-06");
  delay(2000);
  lcd.setCursor(0,2);
  lcd.print("AIR PURIFIER");
  lcd.setCursor(0,3);
  lcd.print("SYSTEM READY....");
  delay(3000);
  lcd.clear();
  

pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT); 
pinMode(measurePin,INPUT); 
pinMode(measurePin1,INPUT); 
pinMode(ledPower,OUTPUT);
pinMode(ledPower1,OUTPUT);
  Serial.begin(115200);
 Blynk.begin(auth, ssid, pass);
}


void loop() {
  
  Blynk.run();
  // put your main code here, to run repeatedly:
  float a = d1();
  Serial.println(a);
   float b = d2();
   Blynk.virtualWrite(V0,a);
   Blynk.virtualWrite(V1,b);
   Serial.println(b);
   lcd.clear();
   delay(300);
  lcd.print("DUST LVL   |");
   lcd.setCursor(0,1);
  lcd.print(b);
  lcd.setCursor(4,1);
  lcd.print("  ug/m3|");
  lcd.setCursor(0,2);
  lcd.print("AIR OUT    |");
  lcd.setCursor(0,3);
  lcd.print(a);
  lcd.setCursor(4,3);
  lcd.print("  ug/m3|");
  
  
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
   dcfanspeed();
//    ledcWrite(pwmChannel, 10);
  
}



int d1(){
  
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = (0.17*calcVoltage-0.1)*100;
  
  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  delay(100);
  return dustDensity;
  }
  
  int d2(){

  digitalWrite(ledPower1,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin1);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower1,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = (0.17*calcVoltage-0.1)*100;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity);
 

  delay(100);
  return dustDensity;
  
  }
