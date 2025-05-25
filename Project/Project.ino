#include <Servo.h>

#define TRIG_PIN 3     // D3 - HC-SR04 Trig
#define ECHO_PIN 2     // D2 - HC-SR04 Echo

#define IR1_PIN 8      // D8 - IR Sensör 1
#define IR2_PIN 9      // D9 - IR Sensör 2

#define SERVO1_PIN 13  // D13 - Servo Motor 1
#define SERVO2_PIN 12  // D12 - Servo Motor 2

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600);
  Serial.println("Park Sistemi Başlatılıyor...");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  servo1.write(0);  // Başlangıçta kapalı
  servo2.write(0);

  Serial.println("Hazır. Sensörler kontrol ediliyor...");
  Serial.println("----------------------------------------");
}

long measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // max 30 ms
  if (duration == 0) return -1;  // Ölçüm başarısız

  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  int ir1 = digitalRead(IR1_PIN);  // LOW = engel var
  int ir2 = digitalRead(IR2_PIN);
  long distance = measureDistanceCm();

  Serial.println("----- Yeni Okuma -----");

  if(distance != -1) {
    Serial.print("Mesafe: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Mesafe ölçülemedi");
  }

  // IR1 ve mesafe kontrolü ile Servo1
  if (ir1 == LOW) {
    Serial.println("IR1: Engel ALGILANDI -> Servo1 DÖNMEZ");
    servo1.write(0);
  } else {
    Serial.println("IR1: Engel YOK");
    if (distance != -1 && distance < 10) {
      Serial.println("Mesafe < 10 cm -> Servo1 DÖNMEZ");
      servo1.write(0);
    } else {
      Serial.println("Mesafe > 10 cm -> Servo1 90 DERECE DÖNER");
      servo1.write(90);
    }
  }

  // IR2 kontrolü ile Servo2
  if (ir2 == LOW) {
    Serial.println("IR2: Engel ALGILANDI -> Servo2 90 DERECE DÖNER");
    servo2.write(90);
  } else {
    Serial.println("IR2: Engel YOK -> Servo2 0 DERECE");
    servo2.write(0);
  }

  Serial.println("------------------------\n");
  delay(1000);  // 1 saniyede bir okuma
}
