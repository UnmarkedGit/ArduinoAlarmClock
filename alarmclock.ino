
#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

DS3231 clock1;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define buzzer 8

// Constants for button pins
#define BT_TIME A0
#define BT_UP A1
#define BT_DOWN A2
#define BT_ALARM A3

enum AlarmStatus {
  ALARM_DEACTIVATED,
  ALARM_ACTIVATED
};

int setMode = 0, mode = 0, setAlarm = 0, alarmMode=0;

int hh = 0, mm = 0, ss = 0;
int ahh = 0, amm = 0, ass = 0;
bool h12Flag = false, pmFlag = false;
int AlarmHH = 21, AlarmMM = 22, AlarmSS = 23;
AlarmStatus alarmStatus = ALARM_DEACTIVATED;
bool flag = false;

void setup() {
  Serial.begin(9600);
	Wire.begin();
  pinMode(BT_TIME, INPUT_PULLUP);
  pinMode(BT_UP, INPUT_PULLUP);
  pinMode(BT_DOWN, INPUT_PULLUP);
  pinMode(BT_ALARM, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" Real Time Clock ");
  lcd.setCursor(0, 1);
  lcd.print("   With Alarm ");
  delay(2000);
  lcd.clear();
  ReadEeprom();
}

void loop() {
hh = clock1.getHour(h12Flag, pmFlag);
  mm = clock1.getMinute();
  ss = clock1.getSecond();
    if(hh>23)hh=0;
    if(mm>59)mm=0;
    if(ss>59)ss=0;
    if(ahh>23)ahh=0;
    if(amm>59)amm=0;
    if(ass>59)ass=0;

    if(hh == -1)hh=23;
    if(mm == -1)mm=59;
    if(ss == -1)ss=59;
    if(ahh == -1)ahh=23;
    if(amm == -1)amm=59;
    if(ass == -1)ass=59;

    lcd.setCursor(0, 0);
    lcd.print((hh / 10) % 10);
    lcd.print(hh % 10);
    lcd.print(":");
    lcd.print((mm / 10) % 10);
    lcd.print(mm % 10);
    lcd.print(":");
    lcd.print((ss / 10) % 10);
    lcd.print(ss % 10);
    lcd.print(" ");
    lcd.print("AlOn");
    
    lcd.setCursor(0, 1);
    lcd.print((ahh / 10) % 10);
    lcd.print(ahh % 10);
    lcd.print(":");
    lcd.print((amm / 10) % 10);
    lcd.print(amm % 10);
    lcd.print(":");
    lcd.print((ass / 10) % 10);
    lcd.print(ass % 10);
    lcd.print(" ");
    if (mode == 1) {
      lcd.write("Y");
    } else {
      lcd.print("N");
    }

  checkAlarm(); 
  controls();
  delay(100);
  blinking();
}

void setAlarmTime() {
  lcd.clear();
  setAlarm = 1;
  while (setAlarm <= 4) {
    lcd.setCursor(0, 0);
    lcd.print("Set Alarm: ");
    lcd.setCursor(0, 1);
    lcd.print(AlarmHH, DEC);
    lcd.print(":");
    lcd.print(AlarmMM, DEC);
    lcd.print(":");
    lcd.print(AlarmSS, DEC);
    if (setAlarm == 1) {
      lcd.setCursor(0, 0);
      lcd.print(">Hour");
    }
    if (setAlarm == 2) {
      lcd.setCursor(3, 0);
      lcd.print(">Minute");
    }
    if (setAlarm == 3) {
      lcd.setCursor(6, 0);
      lcd.print(">Second");
    }
    if (setAlarm == 4) {
      lcd.setCursor(0, 1);
      lcd.print(">Finish");
    }
  }
  setAlarm = 0;
}

void controls(void) {
 if(digitalRead (BT_TIME) == 0 ) {
  setMode++;
  if(setMode == 6 ){
    setMode = 0;
  }
 }
  
 if(digitalRead (BT_ALARM) == 0 ){
   if(mode == 1){
     mode = 0;
   }else{
     mode = 1;
   }
 } 

  
 if(digitalRead (BT_UP) == 0){     
if ( setMode == 0){hh++;clock1.setSecond(hh);}
if ( setMode == 1){mm++;clock1.setMinute(mm);}
if ( setMode == 2){ss++;clock1.setHour(ss);}
if (setMode == 3){ahh++;}
if ( setMode == 4){amm++;}
if (setMode == 5){ass++;}
 }        

if(digitalRead (BT_DOWN) == 0){
if ( setMode == 0){hh--;clock1.setSecond(hh);}
if ( setMode == 1){mm--;clock1.setMinute(mm);}
if ( setMode == 2){ss--;clock1.setHour(ss);}
if (setMode == 3){ahh--;}
if ( setMode == 4){amm--;}
if (setMode == 5){ass--;}
 }
 
}

void blinking (){
//BLINKING SCREEN
if ( setMode == 1){lcd.setCursor(0,0);  lcd.print("  ");}
if ( setMode == 2){lcd.setCursor(3,0);  lcd.print("  ");}
if ( setMode == 3){lcd.setCursor(6,0);  lcd.print("  ");}
if (setMode == 4){lcd.setCursor(1,1);  lcd.print("   ");}
if ( setMode == 5){lcd.setCursor(5,1);  lcd.print("  ");}
if (setMode == 0){lcd.setCursor(8,1);  lcd.print("  ");}
}

void checkAlarm() {
  if (mode == 1 && hh == ahh && mm == amm && ss >= ass) {
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
  }
  else{
    digitalWrite(buzzer, LOW);
  }
}

void ReadEeprom () {
  AlarmHH=EEPROM.read(1);
  AlarmMM=EEPROM.read(2);
  AlarmSS=EEPROM.read(3);
  
  mode=EEPROM.read(4); 
}
void WriteEeprom () {
  EEPROM.write(1,AlarmHH);
  EEPROM.write(2,AlarmMM);
  EEPROM.write(3,AlarmSS);
  
  EEPROM.write(4,mode);
}
