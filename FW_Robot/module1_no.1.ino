/*RFID태그 인식되면 배열에 저장*/
#include<SPI.h>
#include<MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9


MFRC522 rfid(SS_PIN, RST_PIN);


byte bookID[3];
int speakerpin = 5; //스피커가 연결된 디지털핀 설정

void setup() {
    Serial.begin(9600);
    SPI.begin(); //SPI통신 초기화
    rfid.PCD_Init(); //MFRC 초기화
    Serial.print(F("시작"));
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent()) {//만약 새카드가 입력된다면
        return;
    }
    if (!rfid.PICC_ReadCardSerial()) {//만약 읽었던 카드가 입력된다면
        return;
    }

    if (rfid.uid.uidByte[0] != bookID[0] || rfid.uid.uidByte[1] != bookID[1] || rfid.uid.uidByte[2] != bookID[2] || rfid.uid.uidByte[3] != bookID[3])
    {
        Serial.println(F("새로운 책이 들어왔습니다."));
        tone(speakerpin, 500, 1000);  //500:음의 높낮이, 1000:지속시간(1초)
        delay(500);

        for (byte i = 0; i <= 2; i++) {
            bookID[i] = rfid.uid.uidByte[i];
        }
        Serial.print(F("인식된 도서 태그 번호는"));
        printDec(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
    }
  
    else {
        Serial.println(F("이미 IBA가 책을 담고있어요."));
        tone(speakerpin, 1000, 1000);
        delay(500);
    }
}

void printDec(byte* buffer, byte bufferSize) {//10진수 출력함수
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? "0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
