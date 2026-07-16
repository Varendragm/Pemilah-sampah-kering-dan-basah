#include <Servo.h>

Servo servoSortir;

// Ultrasonik
const int trigPin = 9;
const int echoPin = 8;

// Moisture
const int moisturePin = A0;
const int batasBasah = 500;

// Servo angle
const int kiri   = 30;
const int kanan  = 150;
const int tengah = 90;

// Status sistem
bool sedangMemilah = false;

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  servoSortir.attach(6);
  servoSortir.write(tengah);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  // ===== Baca Ultrasonik =====
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000); // timeout 25 ms
  if (duration == 0) return;

  distance = duration * 0.034 / 2;
  Serial.print("Jarak: ");
  Serial.println(distance);

  // ===== Trigger hanya sekali =====
  if (distance < 20 && !sedangMemilah) {
    sedangMemilah = true;

    Serial.println("Objek terdeteksi, menunggu 3 detik...");
    delay(3000); // ⏳ tunggu 3 detik sebelum servo bergerak

    // ===== Baca moisture (rata-rata) =====
    int total = 0;
    for (int i = 0; i < 5; i++) {
      total += analogRead(moisturePin);
      delay(50);
    }
    int nilai = total / 5;

    Serial.print("Moisture AVG: ");
    Serial.println(nilai);

    // ===== Keputusan =====
    if (nilai < batasBasah) {
      servoSortir.write(kiri);
      Serial.println("HASIL: BASAH → KIRI");
    } else {
      servoSortir.write(kanan);
      Serial.println("HASIL: KERING → KANAN");
    }

    delay(1500); // waktu sampah lewat

    servoSortir.write(tengah);
    delay(1000);

    sedangMemilah = false;
    Serial.println("Sistem siap kembali");
  }
}
