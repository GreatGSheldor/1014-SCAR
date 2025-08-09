#include <Servo.h>

Servo servos[4];

const int servoPins[4] = {3, 5, 6, 9};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(90);
  }
  Serial.println("Send commands like: s1 90");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() < 3) return;
    
    if (input.charAt(0) == 's') {
      int servoNum = input.charAt(1) - '1';
      if (servoNum < 0 || servoNum > 3) {
        Serial.println("Servo number wrong");
        return;
      }
      int spaceIndex = input.indexOf(' ');
      if (spaceIndex == -1) {
        Serial.println("Wrong format, use: sX angle");
        return;
      }
      String angleStr = input.substring(spaceIndex + 1);
      int angle = angleStr.toInt();
      if (angle < 0 || angle > 180) {
        Serial.println("Angle must be 0 to 180");
        return;
      }
      servos[servoNum].write(angle);
      Serial.print("Servo ");
      Serial.print(servoNum + 1);
      Serial.print(" moved to ");
      Serial.println(angle);
    }
  }
}
