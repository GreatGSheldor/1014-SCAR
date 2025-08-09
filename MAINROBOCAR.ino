#include <AFMotor.h>
#include <Servo.h>
#include "DHT.h"

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

Servo servo1;
Servo servo2;

#define MQ2_PIN A8
#define DHTPIN A9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("System starting...");

  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);

  servo1.attach(9);
  servo2.attach(10);
  servo1.write(90);
  servo2.write(90);

  dht.begin();

  Serial.println("Ready:");
  Serial.println("Motors: f,b,l,r,s");
  Serial.println("Servo control: s1 <angle>, s2 <angle>");
  Serial.println("Sensors: t (DHT), q (MQ2)");
}

void loop() {
  int gasValue = analogRead(MQ2_PIN);
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 1) {
      char cmd = input.charAt(0);
      switch (cmd) {
        case 'f':
          setAllMotors(FORWARD, 200);
          Serial.println("Motors -> FORWARD");
          break;
        case 'b':
          setAllMotors(BACKWARD, 200);
          Serial.println("Motors -> BACKWARD");
          break;
        case 'l':
          motor1.setSpeed(200); motor1.run(BACKWARD);
          motor3.setSpeed(200); motor3.run(BACKWARD);
          motor2.setSpeed(200); motor2.run(FORWARD);
          motor4.setSpeed(200); motor4.run(FORWARD);
          Serial.println("Motors -> LEFT");
          break;
        case 'r':
          motor1.setSpeed(200); motor1.run(FORWARD);
          motor3.setSpeed(200); motor3.run(FORWARD);
          motor2.setSpeed(200); motor2.run(BACKWARD);
          motor4.setSpeed(200); motor4.run(BACKWARD);
          Serial.println("Motors -> RIGHT");
          break;
        case 's':
          stopAllMotors();
          Serial.println("Motors -> STOP");
          break;
        case 't':
          printDHT(temp, hum);
          break;
        case 'q':
          Serial.print("MQ2 raw: ");
          Serial.println(gasValue);
          break;
        default:
          Serial.println("Unknown command");
          break;
      }
    }
    else if (input.startsWith("s1") || input.startsWith("s2")) {
      int servoNum = (input.charAt(1) == '1') ? 1 : 2;
      int spaceIndex = input.indexOf(' ');
      if (spaceIndex > 0) {
        String angleStr = input.substring(spaceIndex + 1);
        angleStr.trim();
        int angle = angleStr.toInt();
        if (angle >= 0 && angle <= 180) {
          if (servoNum == 1) {
            servo1.write(angle);
            Serial.print("Servo1 set to ");
            Serial.println(angle);
          } else {
            servo2.write(angle);
            Serial.print("Servo2 set to ");
            Serial.println(angle);
          }
        } else {
          Serial.println("Angle must be between 0 and 180");
        }
      } else {
        Serial.println("Invalid servo command format. Use 's1 <angle>'");
      }
    } else {
      Serial.println("Invalid command");
    }
  }
  delay(100);
}

void setAllMotors(uint8_t dir, int speed) {
  motor1.setSpeed(speed); motor1.run(dir);
  motor2.setSpeed(speed); motor2.run(dir);
  motor3.setSpeed(speed); motor3.run(dir);
  motor4.setSpeed(speed); motor4.run(dir);
}

void stopAllMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void printDHT(float temp, float hum) {
  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT read failed");
  } else {
    Serial.print("Temperature: "); Serial.print(temp); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
  }
}
