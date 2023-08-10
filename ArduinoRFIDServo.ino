#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10  // Slave Select pin
#define RST_PIN 9  // Reset pin
#define PIN_SERVO 6 // Pin Servo


MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; 

// Array untuk menyimpan UID kartu yang terdaftar
String registeredCards[10];  // Sesuaikan ukurannya sesuai kebutuhan
int registeredCount = 0;

bool registeringCard = false;
String newCardUID = "";

void setup() {
  Serial.begin(9600);
  Serial.println("HUMANIKA RFID Card Registration System");
  Serial.println("STATUS: Initalizing system ...");
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(PIN_SERVO);
}

void loop() {

  // run all function (prompt/checker/registering)
  promptRegisterCard();
  promptCheckCard();
  checkCard();
  registerCard();
}

// Memutar servo
void rollServo() {

  int pos = 0;
  Serial.println("STATUS: Memutar servo...");
  for (pos = 0; pos <= 120; pos++) {
    myServo.write(pos);
    delay(15);
  }  
  if (pos >= 120) {
    delay(1000);
    for (pos = 120; pos >= 0; pos--) {
      myServo.write(pos);
      delay(15);
    }      
  }
  Serial.println("STATUS: Servo telah diputar.");  
}

// Daftar kartu baru melalui Serial Monitor
void promptRegisterCard(){
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      if (input.equalsIgnoreCase("register")) {
        registeringCard = true;
        newCardUID = "";
        Serial.println("MODE: Pendaftaran kartu");
        Serial.println("Silahkan scan kartu yang ingin didaftarkan:");
      }
    }
  }
}

// Cek kartu melalui Serial Monitor
void promptCheckCard() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      if (input.equalsIgnoreCase("cek")) {
        registeringCard = false;
        newCardUID = "";
        Serial.println("MODE: Pengecekan kartu");
        Serial.println("Silahkan scan kartu...");
      }
    }
  }  
}

// Pengecekan kartu RFID terdaftar
void checkCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && !registeringCard) {
    Serial.println("STATUS: Kartu terdeteksi!");

    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }

    if (isRegistered(cardUID)) {
      Serial.println("STATUS: Kartu terdaftar.");
      rollServo();
    } else {
      Serial.println("STATUS: Kartu tidak terdaftar.");
      Serial.println("TIPS: Jika ingin mendaftarkan kartu, silahkan ketik register di inputan serial monitor");
      delay(500);
    }
  }  
}


// Mendaftarkan kartu baru melalui RFID
void registerCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && registeringCard) {
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }    
    if (isRegistered(cardUID)) {
      Serial.println("ERROR: Kartu ini sudah terdaftar.");
      delay(500);
    }
    else {
      if (registeredCount < sizeof(registeredCards) / sizeof(registeredCards[0])) {
          registeredCards[registeredCount] = cardUID;
          registeredCount++;
  
          Serial.println("STATUS: Kartu berhasil di daftarkan.");
          registeringCard = false;
          cardUID = "";
          Serial.println("MODE: Pengecekan kartu");
          Serial.println("Silahkan scan kartu...");
          delay(500);
      } else {
          Serial.println("STATUS: Kartu gagal didaftarkan, sudah mencapai kapasitas maksimal.");
          delay(500);
      }          
    }
  }  
}

bool isRegistered(String cardUID) {
  for (int i = 0; i < registeredCount; i++) {
    if (cardUID == registeredCards[i]) {
      return true;
    }
  }
  return false;
}
