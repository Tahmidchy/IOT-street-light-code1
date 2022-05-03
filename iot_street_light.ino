#include <Robojax_AllegroACS_Current_Sensor.h>
#include <LiquidCrystal.h>
#include <SoftPWM.h>

#define OFF   0
#define ON  255
#define MID  30

Robojax_AllegroACS_Current_Sensor robojax(0, A7);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

byte lights[] = {6, 5, 4, 3, 2, 13};
byte duty[] = {OFF, OFF, OFF, OFF, OFF, OFF};
int dayNight, lumens[6], amp;
bool timeover, serverMode, distance[6];
byte sec, i, count, duration, temp;
long prevMs;

byte off[] = {0x04, 0x0A, 0x11, 0x11, 0x11, 0x11, 0x0E, 0x00};
byte on[] = {0x04, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x00};
byte mid[] = {0x04, 0x0E, 0x1F, 0x1F, 0x11, 0x11, 0x0E, 0x00};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, off);
  lcd.createChar(1, on);
  lcd.createChar(2, mid);
  SoftPWMBegin();

  for (byte i = 0; i < 6; i++) {
    pinMode(lights[i], OUTPUT);
    SoftPWMSet(lights[i], duty[i]);
    SoftPWMSetFadeTime(lights[i], 10, 10);
  }
  lcd.print(F("WIFI INIT..."));
  delay(5000);
}

void loop() {
  amp = robojax.getCurrentAverage(300) * 1000;
  dayNight = analogRead(A6);
  dayNight = map(dayNight, 0, 1023, 0, 100);
  lightControl();

  if (millis() - prevMs >= 1000) {
    if (serverMode == 0 && duration > 0) {
      if (sec != duration) sec++;
      else if (sec == duration) timeover = 1;
    }
    else sec = 0;

    count++;
    if (count == 10) {
      count = 0;
      lcd.clear();
      lcd.print(F("UPDATING SERVER"));
      String cmd1 = (String)"a=" + amp
                    + "&s=" + dayNight
                    + "&l1=" + (duty[0] == 0 ? "0" : "1") + "@" + lumens[0]
                    + "&l2=" + (duty[1] == 0 ? "0" : "1") + "@" + lumens[1];
      String cmd2 = (String)"&l3=" + (duty[2] == 0 ? "0" : "1") + "@" + lumens[2]
                    + "&l4=" + (duty[3] == 0 ? "0" : "1") + "@" + lumens[3]
                    + "&l5=" + (duty[4] == 0 ? "0" : "1") + "@" + lumens[4]
                    + "&l6=" + (duty[5] == 0 ? "0" : "1") + "@" + lumens[5];
      // a=0.02&s=341&l1=0@736&l2=0@709&l3=0@851&l4=0@742&l5=0@227&l6=0@812$
      Serial.println(cmd1 + cmd2 + "$");
      delay(2000);
      lcd.clear();
    }

    lcd.setCursor(0, 0);
    lcd.print((String)amp + "mA,LIGHT:" + dayNight + "%  ");
    lcd.setCursor(0, 1);
    if (serverMode == 0 && timeover == 0)
      lcd.print((String)"COUNT: " + sec + " to " + duration + "  ");
    else if (serverMode == 1) lcd.print(F("SERVER CONTROL=1"));

    prevMs = millis();
  }

  if (Serial.available()) {
    String cmd = Serial.readString();

    if (cmd.indexOf("S=") != -1) {
      cmd.remove(0, 2);
      i = cmd.indexOf("$");
      cmd.remove(i);

      for (byte n = 0; n < 6; n++) {
        duty[n] = cmd.toInt();
        if (duty[n] == 0) duty[n] = OFF;
        else duty[n] = ON;
        i = cmd.indexOf(",");
        cmd.remove(0, i + 1);
      }
      serverMode = 1;
      timeover = 0;
    }
    else if (cmd.indexOf("D=") != -1) {
      cmd.remove(0, 2);
      i = cmd.indexOf("$");
      cmd.remove(i);

      for (byte n = 0; n < 7; n++) {
        if (n == 0) duration = cmd.toInt();
        else distance[n - 1] = cmd.toInt();
        i = cmd.indexOf(",");
        cmd.remove(0, i + 1);
      }
      serverMode = 0;
    }
    Serial.flush();
    cmd = "";
  }
}

void lightControl() {
  for (byte i = 0; i < 6; i++) {
    if (serverMode == 0) {
      if (dayNight < 50) {
        if (timeover == 1) {
          if (distance[i] == 1) duty[i] = ON;
          else duty[i] = MID;
        }
        else duty[i] = ON;
      }
      else duty[i] = OFF;

      if (timeover == 1) { 
        lcd.setCursor(i * 2, 1);
        if (duty[i] == OFF) lcd.write(byte(0));
        else if (duty[i] == ON) lcd.write(byte(1));
        else if (duty[i] == MID) lcd.write(byte(2));
        if(i != 5) lcd.print("_");
        else lcd.print(F(" MANL"));
      }
    }
    SoftPWMSet(lights[i], duty[i]);
    lumens[i] = analogRead(14 + i);
    lumens[i] = map(lumens[i], 0, 1023, 0, 99);
  }
}
