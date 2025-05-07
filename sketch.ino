#include <LiquidCrystal_I2C.h>  // Library untuk LCD I2C
#include <Servo.h>              // Library untuk motor servo

// Definisi pin
#define BUZZER 3          // Pin untuk buzzer
#define LEDHIJAU 4        // Pin untuk LED hijau (indikator normal)
#define LEDMERAH 5        // Pin untuk LED merah (indikator api)
#define SERVOPIN 9        // Pin untuk motor servo
#define RELAY 10          // Pin untuk relay (pemadam api)
#define LDRPIN A0         // Pin untuk sensor LDR (sensor cahaya)

LiquidCrystal_I2C lcd(0x27, 12, 2); // Inisialisasi LCD I2C (alamat 0x27, ukuran 20x4)
Servo servo;                        // Objek untuk mengendalikan servo

void setup() {
  // Inisialisasi pin sebagai output
  pinMode(LEDMERAH, OUTPUT);
  pinMode(LEDHIJAU, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Inisialisasi LCD
  lcd.init();           
  lcd.backlight();      // Nyalakan lampu latar LCD
  lcd.clear();          // Bersihkan layar LCD
  lcd.setCursor(2, 0);  
  lcd.print("PEMADAM API");   // Judul tampilan
  lcd.setCursor(3, 1);  
  lcd.print("OTOMATIS");      // Subjudul
  delay(2000);         // Tampilkan selama 2 detik

  // Set kondisi awal: semua perangkat mati
  digitalWrite(LEDMERAH, LOW);
  digitalWrite(LEDHIJAU, LOW);
  noTone(BUZZER);
  digitalWrite(RELAY, LOW);

  // Inisialisasi servo ke posisi awal
  servo.attach(SERVOPIN);
  servo.write(0);
}

void loop() {
  int lightLevel = analogRead(LDRPIN); // Baca nilai cahaya dari LDR (0 = terang, 1023 = gelap)

  // Jika cahaya sangat terang (nilai di bawah 300), diasumsikan ada api
  if (lightLevel < 300) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("API TERDETEKSI");  // Tampilkan status api

    lcd.setCursor(2, 1);
    lcd.print("CAHAYA : ");
    lcd.print(lightLevel);       // Tampilkan nilai cahaya di LCD

    // Aktifkan semua sistem peringatan & pemadaman
    digitalWrite(LEDMERAH, HIGH);  // LED merah menyala
    digitalWrite(LEDHIJAU, LOW);   // LED hijau mati
    digitalWrite(RELAY, HIGH);     // Aktifkan relay (semprot air)
    tone(BUZZER, 1000);            // Buzzer bunyi
    servo.write(90);               // Servo bergerak (misal membuka katup)

    delay(5000);                   // Tunggu 5 detik saat pemadaman

    // Setelah 5 detik, tampilkan status berhasil
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("API BERHASIL");
    lcd.setCursor(3, 1);
    lcd.print("DIPADAMKAN");

    // Kembalikan sistem ke status aman
    digitalWrite(LEDMERAH, LOW);
    digitalWrite(LEDHIJAU, HIGH);
    noTone(BUZZER);
    digitalWrite(RELAY, LOW);
    servo.write(0);               // Servo kembali ke posisi awal

    delay(3000);                  // Tampilkan status berhasil selama 3 detik
  } 
  else {
    // Jika tidak ada api (cahaya normal), tampilkan info sistem
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("TIDAK ADA API");

    // Tampilkan nilai cahaya pada baris berikutnya
    lcd.setCursor(2, 1);  // Baris 2 (jika LCD 20x4, ini baris ketiga)
    lcd.print("CAHAYA : ");
    lcd.print(lightLevel);

    delay(2000);  // Tampilkan selama 2 detik

    // Pastikan semua sistem peringatan dalam keadaan nonaktif
    digitalWrite(LEDMERAH, LOW);
    digitalWrite(LEDHIJAU, HIGH);
    noTone(BUZZER);
    digitalWrite(RELAY, LOW);
    servo.write(0);
  }

  delay(500);  // Delay untuk pembacaan sensor berikutnya
}