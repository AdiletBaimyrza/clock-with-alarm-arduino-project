#include <Wire.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include "pitches.h"
#include "SR04.h"

#define BUZZER_PIN 8 
#define ECHO_PIN 3
#define TRIG_PIN 2

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distanceSensor;
DateTime now;
RTC_DS3231 rtc;

struct Alarm
{
  int hour = 8;
  int minute = 0;
};

int GameOfThronesMelody[] = {
  NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C4,
  
  NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_C4,
  
  NOTE_G4, NOTE_C4,
  
  NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
  NOTE_DS4, NOTE_D4, NOTE_C4,
  NOTE_G4, NOTE_C4,
  NOTE_DS4, NOTE_F4, NOTE_G4,  NOTE_C4, NOTE_DS4, NOTE_F4,
  
  NOTE_D4,
  NOTE_F4, NOTE_AS3,
  NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_AS3,
  NOTE_C4,
  NOTE_C5,
  NOTE_AS4,
  NOTE_C4,
  NOTE_G4,
  NOTE_DS4,
  NOTE_DS4, NOTE_F4,
  NOTE_G4,
  
  NOTE_C5,
  NOTE_AS4,
  NOTE_C4,
  NOTE_G4,
  NOTE_DS4,
  NOTE_DS4, NOTE_D4,
  NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
  
  REFS0, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5,
  NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5
};
int GameOfThronesDurations[] = {
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  4, 4,
  
  16, 16, 4, 4, 16, 16,
  1,
  4, 4,
  16, 16, 4, 4,
  16, 16, 1,
  
  4, 4,
  
  16, 16, 4, 4, 16, 16,
  1,
  4, 4,
  16, 16, 4, 4,
  16, 16, 1,
  4, 4,
  16, 16, 4, 4, 16, 16,
  
  2,
  4, 4,
  8, 8, 8, 8,
  1,
  2,
  2,
  2,
  2,
  2,
  4, 4,
  1,
  
  2,
  2,
  2,
  2,
  2,
  4, 4,
  8, 8, 16, 16, 8, 8, 16, 16,
  8, 8, 16, 16, 8, 8, 16, 16,
  
  4, 16, 16, 8, 8, 16, 16,
  8, 16, 16, 16, 8, 8, 16, 16
};

int HarryPotterMelody[] = {
  REFS0, NOTE_D4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_C5, 
  NOTE_A4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_F4, NOTE_GS4,
  NOTE_D4, 
  NOTE_D4,
  
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_B4,
  NOTE_G4,
  NOTE_AS4,
   
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_DS5, NOTE_D5,
  NOTE_CS5, NOTE_A4,
  NOTE_AS4, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_D4,
  NOTE_D5, 
  REFS0, NOTE_AS4,  
  
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_AS4,
  NOTE_G4
};
int HarryPotterDurations[] = {
  2, 4,
  4, 8, 4,
  2, 4,
  2, 
  2,
  4, 8, 4,
  2, 4,
  1, 
  4,
  
  4, 8, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1,
  4,
   
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1, 
  4, 4,  
  
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1
};

int PiratesOfTheCaribbeanMelody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REFS0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_A4, NOTE_G4, NOTE_A4, REFS0,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REFS0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_A4, NOTE_G4, NOTE_A4, REFS0,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REFS0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REFS0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REFS0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REFS0,
  
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_D5, NOTE_E5, NOTE_A4, REFS0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_C5, NOTE_A4, NOTE_B4, REFS0,
  
  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_A4, NOTE_G4, NOTE_A4, REFS0,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REFS0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_A4, NOTE_G4, NOTE_A4, REFS0,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REFS0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REFS0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REFS0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REFS0,
  
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REFS0,
  NOTE_D5, NOTE_E5, NOTE_A4, REFS0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REFS0,
  NOTE_C5, NOTE_A4, NOTE_B4, REFS0,
  //End of Repeat
  
  NOTE_E5, REFS0, REFS0, NOTE_F5, REFS0, REFS0,
  NOTE_E5, NOTE_E5, REFS0, NOTE_G5, REFS0, NOTE_E5, NOTE_D5, REFS0, REFS0,
  NOTE_D5, REFS0, REFS0, NOTE_C5, REFS0, REFS0,
  NOTE_B4, NOTE_C5, REFS0, NOTE_B4, REFS0, NOTE_A4,
  
  NOTE_E5, REFS0, REFS0, NOTE_F5, REFS0, REFS0,
  NOTE_E5, NOTE_E5, REFS0, NOTE_G5, REFS0, NOTE_E5, NOTE_D5, REFS0, REFS0,
  NOTE_D5, REFS0, REFS0, NOTE_C5, REFS0, REFS0,
  NOTE_B4, NOTE_C5, REFS0, NOTE_B4, REFS0, NOTE_A4
};
int PiratesOfTheCaribbeanDurations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,
  
  4, 8,
  //Repeat of First Part
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,
  //End of Repeat
  
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2,
  
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2
};

int NokiaMelody[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};
int NokiaDurations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};

struct MelodyData {
  const int* melody;
  const int* durations;
  int length;
};

MelodyData gameOfThronesMelodyData = {
  GameOfThronesMelody,
  GameOfThronesDurations,
  sizeof(GameOfThronesMelody) / sizeof(GameOfThronesMelody[0])
};
MelodyData NokiaMelodyData = {
  NokiaMelody,
  NokiaDurations,
  sizeof(NokiaMelody) / sizeof(NokiaMelody[0])
};
MelodyData harryPotterMelodyData = {
  HarryPotterMelody,
  HarryPotterDurations, 
  sizeof(HarryPotterMelody) / sizeof(HarryPotterMelody[0])
};
MelodyData piratesOfTheCaribbeanMelodyData = {
  PiratesOfTheCaribbeanMelody,
  PiratesOfTheCaribbeanDurations,
  sizeof(PiratesOfTheCaribbeanMelody) / sizeof(PiratesOfTheCaribbeanMelody[0])
};


String WEEKDAYS[7] = {
  "SUN",
  "MON",
  "TUE",
  "WED",
  "THU",
  "FRI",
  "SAT",
};

const int rs = 7, en = 13, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte alarm[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100
};
bool alarmRinging = true;
bool alarmStart = false;

void displayDate(void);
void displayTime(void);
void displayAlarm(void);
void ringAlarm(const MelodyData& melodyData);

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT); 
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, alarm);

  Wire.begin();
  rtc.begin();

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (!rtc.begin())
  {
    Serial.println(" RTC Module not Present");
    while (1);
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC power failure, reset the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop()
{
  Alarm a;
  a.hour = 16;
  a.minute = 46;
  now = rtc.now();
  displayDate();
  displayTime();
  displayAlarm(a);

  distanceSensor=sr04.Distance();
   Serial.print(distanceSensor);
   Serial.println("cm");
   delay(50);
  if (distanceSensor < 150) alarmRinging = false;
  if (now.hour() == a.hour && now.minute() == a.minute) 
    alarmStart = true;
  if (alarmRinging && alarmStart) ringAlarm(gameOfThronesMelodyData);
  

}

void displayDate()
{
  lcd.setCursor(0, 0);
  if (now.day() < 10) lcd.print(0);
  lcd.print(now.day());
  lcd.print('/');
  if (now.month() < 10) lcd.print(0);
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
  lcd.setCursor(13, 0);
  lcd.print(WEEKDAYS[now.dayOfTheWeek()]);
}

void displayTime()
{
  lcd.setCursor(0, 1);
  if (now.hour() < 10) lcd.print(0);
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) lcd.print(0);
  lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10) lcd.print(0);
  lcd.print(now.second());
  lcd.print("    ");
}

void displayAlarm(Alarm &a)
{
  lcd.setCursor(10, 1);

  lcd.write(byte(0));
  if (a.hour < 10) lcd.print(0);
  lcd.print(a.hour);
  lcd.print(':');
  if (a.minute < 10) lcd.print(0);
  lcd.print(a.minute);
}

void ringAlarm(const MelodyData& melodyData) {
  for (int note = 0; note < melodyData.length; note++) {
    int duration = 1000 / melodyData.durations[note];
    tone(BUZZER_PIN, melodyData.melody[note], duration);
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}


