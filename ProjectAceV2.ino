/*
   ACE V2
   Created By : RansomBroker
   Website : ransombroker.net
   GitHub : https://github.com/RansomBroker
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <DS3231.h>
#define OLED_RESET 4

RTC_DS3231 rtc;

Adafruit_SSD1306 display(OLED_RESET);

//declare of TimeDate
char daysOfTheWeek[7][4] = {"Min", "Sen", "Sel", "Rab",
                              "Kam", "Jum", "Sab"
                            };
const int button1 = 2;
const int button2 = 3;
const int potentio = A0;
const int outputA1 = A1;
const int buzzer = 11;
String day, month, year, secs, minutes, hours, dofweek, times, dates;
int menu, button1State, button2State, back, select, selectMenuSetting, timeHour, timeMinute,
hourTime, minuteTime;
int addr1 = 0;
int addr2 = 64;
int address1 = 0;
int address2 = 0;
byte hour, minute, valueHour, valueMinute;
//motor
int enA = 9;
int in1 = 6;
int in2 = 7;
int enB = 10;
int in3 = 4;
int in4 = 5;
int rainSensor;
int check = 0;

// 'calendar(1)', 16x8px
const unsigned char calendar [] PROGMEM = {
  0x04, 0x20, 0x0c, 0x30, 0x0f, 0xf0, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x0e, 0x50, 0x0f, 0xf0
};

// 'clock-circular-outline', 16x8px
const unsigned char clock [] PROGMEM = {
  0x03, 0xc0, 0x04, 0x20, 0x08, 0x10, 0x08, 0x10, 0x08, 0xd0, 0x08, 0x10, 0x04, 0x20, 0x03, 0xc0
};

// 'icon', 16x16px
const unsigned char homeMenu [] PROGMEM = {
  0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x06, 0x64, 0x0d, 0xb4, 0x1b, 0xd8, 0x27, 0xe4, 0x5f, 0xf2, 
  0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x00, 0x00
};


// 'icon', 16x16px
const unsigned char onClickHome [] PROGMEM = {
  0xff, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf9, 0x9b, 0xf2, 0x4b, 0xe4, 0x27, 0xd8, 0x1b, 0xa0, 0x0d, 
  0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83, 0xff, 0xff
};

// 'left-arrow', 16x8px
const unsigned char backBtn [] PROGMEM = {
  0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00
};


// 'left-arrow(1)', 16x10px
const unsigned char leftBtn [] PROGMEM = {
  0x00, 0x60, 0x00, 0xe0, 0x01, 0xc0, 0x03, 0x80, 0x07, 0x00, 0x07, 0x00, 0x03, 0x80, 0x01, 0xc0, 
  0x00, 0xe0, 0x00, 0x60
};

// 'right-arrow', 16x10px
const unsigned char rightBtn [] PROGMEM = {
  0x06, 0x00, 0x07, 0x00, 0x03, 0x80, 0x01, 0xc0, 0x00, 0xe0, 0x00, 0xe0, 0x01, 0xc0, 0x03, 0x80, 
  0x07, 0x00, 0x06, 0x00
};

// 'settings', 32x18px
const unsigned char settingIcon [] PROGMEM = {
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x36, 0x6c, 0x00, 0x00, 0x48, 0x12, 0x00, 
  0x00, 0x40, 0x02, 0x00, 0x00, 0x23, 0xc4, 0x00, 0x00, 0x46, 0x62, 0x00, 0x01, 0xcc, 0x33, 0x80, 
  0x01, 0x08, 0x10, 0x80, 0x01, 0x08, 0x10, 0x80, 0x01, 0xcc, 0x33, 0x80, 0x00, 0x46, 0x62, 0x00, 
  0x00, 0x23, 0xc4, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x48, 0x12, 0x00, 0x00, 0x36, 0x6c, 0x00, 
  0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00
};

// 'info', 32x18px
const unsigned char aboutIcon [] PROGMEM = {
  0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
  0x00, 0x01, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 
  0x00, 0x07, 0xf0, 0x00, 0x00, 0x03, 0xc0, 0x00
};



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();
  
  //rtc.setDate(07, 4, 2018);   //mensetting tanggal 07 april 2018
  //rtc.setTime(22, 00, 00);     //menset jam 22:00:00
  //rtc.setDOW(6);     //menset hari "Sabtu"
 
 
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(outputA1, INPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32, 8);
  display.println("Welcome To");
  display.setCursor(40, 18);
  display.println("ACE V2");
  display.display();
  delay(1300);
  display.clearDisplay();
  display.display();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  button1State = digitalRead(button1);
  menu += button1State;
  switch (menu){
      case 0:
        home();
        display.display();
        display.clearDisplay();
        display.drawBitmap(95, 8, homeMenu, 16, 16, WHITE);
        break;
       case 1:
        display.clearDisplay();
        delay(50);
        display.drawBitmap(95, 8, onClickHome, 16,16, WHITE);
        display.setCursor(100, 25);
        display.println("Menu");
        display.display();
        display.clearDisplay();
        analogWrite(buzzer, 50);
        delay(10);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        menu +=1;
       case 2:
       display.clearDisplay();
       delay(500);
       mainMenu();
       display.display();
    }
}

void home(){
  address1 = addr1;
  valueHour = EEPROM.read(address1);

  address2 = addr2;
  valueMinute = EEPROM.read(address2);

  timeHour = valueHour;
  timeMinute = valueMinute;
  rainSensor = analogRead(outputA1);
  DateTime now = rtc.now();
  day = now.day();
  month = now.month();
  year = now.year();
  secs = now.second();
  hours = now.hour();
  hourTime = now.hour();
  minuteTime = now.minute();
  minutes = now.minute();
  dofweek = daysOfTheWeek[now.dayOfTheWeek()];
  times = hours + ":" + minutes + ":" + secs;
  dates = day + "-" + month + "-" + year;

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(24, 4);
  display.println(valueHour);
  display.setCursor(32, 4);
  display.println(":");
  display.setCursor(42, 4);
  display.println(valueMinute);

  display.drawBitmap(0, 16, clock, 16, 8, WHITE);
  display.setCursor(17, 16);
  display.println(times);
  display.drawBitmap(0, 24, calendar, 16, 8, WHITE);
  display.setCursor(15, 24);
  display.println(dates);
  display.drawLine(85, 0, 85, 32, WHITE);
  display.drawLine(87, 0, 87, 32, WHITE);
  Serial.println(rainSensor);

  if(rainSensor <= 650){
      kondisi1();
    }

  if(rainSensor > 650){
      if(timeHour == hourTime && timeMinute == minuteTime){
          Serial.println("angkat");
          kondisi2();
          delay(500);
        }
    }
  
}

void mainMenu(){
  int selectMenu;
  selectMenu = 0;
  button1State = digitalRead(button1);
  selectMenu += button1State;
  button2State = digitalRead(button2);
  back += button2State;
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.drawBitmap(19, 10, leftBtn, 10, 10, WHITE);
  display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
  display.drawBitmap(100, 10, rightBtn, 10, 10, WHITE);
  if(back == 1){
    display.clearDisplay();
    delay(1);
    display.display();
    analogWrite(buzzer, 50);
    delay(10);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    menu = 0;
    back = 0;
  }
  select = analogRead(potentio);
  if(select >= 610 && select <= 1023){
    display.clearDisplay();
    display.drawBitmap(19, 10, leftBtn, 10, 10, WHITE);
    display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
    display.drawBitmap(100, 10, rightBtn, 10, 10, WHITE);
    
    display.drawBitmap(50, 5, settingIcon, 32, 18, WHITE);
    display.setCursor(42, 24);
    display.println("Settings");
    display.display();
    if(selectMenu == 1){
      analogWrite(buzzer, 50);
      delay(10);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      settingMenu();
    }
  }
  if(select >= 0 && select <= 700){
    display.clearDisplay();
    display.drawBitmap(19, 10, leftBtn, 10, 10, WHITE);
    display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
    display.drawBitmap(100, 10, rightBtn, 10, 10, WHITE);

    display.drawBitmap(50, 5, aboutIcon, 32, 18, WHITE);
    display.setCursor(45, 24);
     display.println("Tentang");
    display.display();
    if(selectMenu == 1){
     analogWrite(buzzer, 50);
     delay(10);
     digitalWrite(buzzer, HIGH);
     delay(100);
     digitalWrite(buzzer, LOW);
     tentangProgram();
    }
  }
}

void settingMenu(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.println("Setting");
  display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
  display.display();
  while(true){
    delay(50);
    selectMenuSetting = 0;
    button1State = digitalRead(button1);
    selectMenuSetting += button1State;
    int select = analogRead(potentio);
    button2State = digitalRead(button2);
    back += button2State;
    if(back == 1){
      display.clearDisplay();
      delay(1);
      display.display();
      analogWrite(buzzer, 50);
      delay(10);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      menu = 0;
      back = 0;
      break;
    }
    if(select >= 800 && select <= 950){
      display.clearDisplay();
      display.setCursor(45, 0);
      display.println("Setting");
      display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
      display.setCursor(0, 10);
      display.println("> atur waktu angkat");
      display.setCursor(0, 16);
      display.println("jalankan jemuran");
      display.display();
      Serial.println(selectMenuSetting);
      if(selectMenuSetting == 1){
       analogWrite(buzzer, 50);
       delay(10);
       digitalWrite(buzzer, HIGH);
       delay(100);
       digitalWrite(buzzer, LOW);  
       selectMenuSetting = 0;
       display.clearDisplay();
       waktuAngkat();
       break;
      }   
    }
    if(select >= 500 && select <= 750){
      display.clearDisplay();
      display.setCursor(45, 0);
      display.println("Setting");
      display.drawBitmap(0, 25, backBtn, 16, 8, WHITE);
      display.setCursor(0, 10);
      display.println("> jalankan jemuran");
      display.display();
      if(selectMenuSetting == 1){
       analogWrite(buzzer, 50);
       delay(10);
       digitalWrite(buzzer, HIGH);
       delay(100);
       digitalWrite(buzzer, LOW);  
       selectMenuSetting = 0;
       display.clearDisplay();
       jemuranControl();
       break;
      }   
    }
  }
}

void waktuAngkat() {
  int ok; 
  analogWrite(buzzer, 50);
  delay(10);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);    
  ok = 0;
  hour = 0;
  minute = 0;
  while(true){
    button1State = digitalRead(button1);
    ok += button1State;
    delay(255);
    if(ok == 0){
      button2State = digitalRead(button2);
      hour += button2State;
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("atur waktu format h/m");
      display.setTextSize(2);
      display.setCursor(36, 16);
      display.println(hour);
      display.setCursor(56, 16);
      display.println(":");
      display.setCursor(76, 16);
      display.println(minute);
      display.display();
      if(hour >= 24){
        hour = 0;
      }
    }
    if(ok == 1){
      button2State = digitalRead(button2);
      minute += button2State;
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("atur waktu format h/m");
      display.setTextSize(2);
      display.setCursor(36, 16);
      display.println(hour);
      display.setCursor(56, 16);
      display.println(":");
      display.setCursor(76, 16);
      display.println(minute);
      display.display();
      if(minute >= 59){
        minute = 0;
      }
    }
   if(ok == 3){
    analogWrite(buzzer, 50);
    delay(10);
    digitalWrite(buzzer, HIGH);
    delay(255);
    digitalWrite(buzzer, LOW);
    display.clearDisplay();
    display.setTextSize(1);    
    display.setCursor(16, 0);
    display.println("waktu telah diset");
    delay(500);
    display.setTextSize(2);
    display.setCursor(36, 16);
    display.println(hour);
    display.setCursor(56, 16);
    display.println(":");
    display.setCursor(76, 16);
    display.println(minute);
    display.display();
    EEPROM.write(addr1, hour);
    EEPROM.write(addr2, minute);
    delay(1300);
    display.clearDisplay();
    mainMenu();
    break;
   }
  }
}

void jemuranControl(){
 int runMotor;
 runMotor = 0; 
 display.clearDisplay();
 display.setTextSize(1);
 display.setCursor(16, 0);
 display.println("Jalankan Jemuran");
 display.drawRect(16, 16, 50, 16, WHITE);
 display.setCursor(24, 20);
 display.println("maju");
 display.drawRect(70, 16, 50, 16, WHITE);
 display.setCursor(76, 20);
 display.println("mundur");
 display.display();
 while(true){
    button1State = digitalRead(button1);
    delay(25);
    button2State = digitalRead(button2);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    back += button2State;
    if(back == 1){
      display.clearDisplay();
      delay(1);
      display.display();
      analogWrite(buzzer, 50);
      delay(10);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      menu = 0;
      back = 0;
      break;
    }
   int select = analogRead(potentio);
   if(select >=600 && select <= 950){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(16, 0);
      display.println("Jalankan Jemuran");
      display.fillRect(16, 16, 50, 16, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(24, 20);
      display.println("maju");
      display.drawRect(70, 16, 50, 16, WHITE);
      display.setTextColor(WHITE);
      display.setCursor(76, 20);
      display.println("mundur");
      display.display();
      if(button1State == 1){
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
      }else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
      }
   }
    if(select >=0 && select <= 600){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(16, 0);
      display.println("Jalankan Jemuran");
      display.drawRect(16, 16, 50, 16, WHITE);
      display.setTextColor(WHITE);
      display.setCursor(24, 20);
      display.println("maju");
      display.fillRect(70, 16, 50, 16, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(76, 20);
      display.println("mundur");
      display.display();
      if(button1State == 1){
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
      }else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
      }
   }
 }
 
}



void kondisi1(){
  analogWrite(buzzer, 50);
  delay(10);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  analogWrite(buzzer, 50);
  delay(10);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, LOW);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(32, 0);
  display.println("Hari Hujan");
  display.display();

  display.clearDisplay();
  delay(1000);

  display.clearDisplay();
  display.setCursor(16, 10);
  display.println("jemuran dia angkat");
  display.display();

  analogWrite(enA, 200);
  analogWrite(enB, 200);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  while(true){
    delay(25);
    rainSensor = analogRead(outputA1);
    if(rainSensor > 660){
      analogWrite(enA, 200);
      analogWrite(enB, 200);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      delay(1500);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);  
      home();
      break;
    }
  }
  
}

void kondisi2(){
  analogWrite(buzzer, 50);
  delay(10);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(24, 0);
  display.println("alarm auto");
  display.display();

  display.clearDisplay();
  display.setCursor(16, 10);
  display.println("jemuran dia angkat");
  display.display();

  analogWrite(enA, 200);
  analogWrite(enB, 200);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1500);
  home();

}

void tentangProgram(){
  int next, select;
  select = 0;
  display.clearDisplay();
  while(true){
    delay(255);
    next = digitalRead(button1);
    select += next; 
    if(select == 0){
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);

      display.setCursor(16, 0);
      display.println("Tentang Program");

      display.setCursor(0, 8);
      display.println("this program was made");
      display.setCursor(0, 16);
      display.println("by RansomBroker 2019");
      display.display();
      
    }
   if(select == 1){
      display.clearDisplay();
      delay(10);
      display.display();
      display.clearDisplay();    
      delay(100);
      back = 0;
      mainMenu();
      break;
     }
  }  
}
