#include <SoftwareSerial.h>
#include <NewPing.h>

#define MAX_DISTANCE 20
#define DELAY 30

SoftwareSerial wifi(A0, A1);

NewPing sonar[] = {
  NewPing(12, A2, MAX_DISTANCE),
  NewPing(10, 11, MAX_DISTANCE),
  NewPing(8, 9, MAX_DISTANCE),
  NewPing(6, 7, MAX_DISTANCE),
  NewPing(4, 5, MAX_DISTANCE),
  NewPing(2, 3, MAX_DISTANCE)
};

int i;
byte distance[6], temp;
float volt;
byte state[8];

void setup() {
  Serial.begin(9600);
  wifi.begin(9600);

  state[1] = DELAY;
}

void loop() {
  int adc = analogRead(A6);
  temp = adc / 15;
  adc = analogRead(A7);
  volt = adc * (5. / 1023);

  if (wifi.available() > 0) {
    String cmd = wifi.readString();  // format: M=<sm>,<time>,<light state>,$
    if (cmd.indexOf("M=") != -1) {
      cmd.remove(0, 2);
      i = cmd.indexOf("$");
      cmd.remove(i);

      for (byte n = 0; n < 8; n++) {
        state[n] = cmd.toInt();
        i = cmd.indexOf(",");
        cmd.remove(0, i + 1);
      }

      if (state[0] == 1) {  // server mode ON
        String msg = "S=";
        for (byte n = 2; n < 8; n++) {
          msg += (String) state[n] + ",";
        }
        Serial.println(msg + "$");  // format: S=<light flag>,$
      }
      else { // server mode OFF
        for (byte i = 0; i < 6; i++) {
          delay(50);
          distance[i] = sonar[i].ping_cm();
          if (distance[i] != 0) distance[i] = 1;
          else distance[i] = 0;
        }

        String msg = (String)"D=" + state[1] + ",";
        for (byte i = 0; i < 6; i++) {
          msg += (String) distance[i] + ",";
        }
        Serial.println(msg + "$");  // format: D=<time>,<distance flag>,$
      }
    }
  }

  // format: <DATA FROM ARDUINO>$
  if (Serial.available() > 0) {
    String cmd = Serial.readString();
    if (cmd.indexOf("a=") != -1) {
      i = cmd.indexOf("$");
      cmd.remove(i);
      String cmd2 = (String)"&t=" + temp + "&v=" + volt;
      wifi.println(cmd + cmd2 + "$");
    }
  }
}
