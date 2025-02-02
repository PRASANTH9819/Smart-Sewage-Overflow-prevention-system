#include <HardwareSerial.h>

HardwareSerial mySerial1(1);
HardwareSerial mySerial2(2);

char msg;
char call;
unsigned char data[4] = {};
float distance;

void setup() {
  Serial.begin(115200); 
  mySerial1.begin(9600, SERIAL_8N1, 4, 5);
  mySerial2.begin(9600, SERIAL_8N1, 18, 19); // RX, TX

  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("s : to send message");
  Serial.println("c : to call");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("e : to redial");
  Serial.println();
  delay(100); 
}

void loop() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 's':
        SendMessage();
        break;
      case 'c':
        MakeCall(); 
        CheckCallStatus(); // Check call status after making the call
        break;
    }
  }

  if (mySerial2.available() > 0)
    Serial.write(mySerial2.read());

  do {
    for (int i = 0; i < 4; i++) {
      data[i] = mySerial1.read();
    }
  } while (mySerial1.available() > 0 && data[0] == 0xFF);

  while (mySerial1.available() > 0) {
    mySerial1.read();
  }

  if (data[0] == 0xFF) {
    int sum = (data[0] + data[1] + data[2]) & 0x00FF;
    if (sum == data[3]) {
      distance = (data[1] << 8) + data[2];
      Serial.print("Distance = ");
      Serial.print(distance / 10);
      Serial.println(" cm");

      if (distance < 300) { 
        MakeCall(); 
        CheckCallStatus();
        SendMessage();
      } else if (distance < 900) { 
        SendMessage1();
      } else if (distance < 1570) { 
        SendMessage2();
        delay(6000); 
      }
    } else {
      Serial.println("Invalid data received");
    }
  } else {
    Serial.println("Invalid data received");
  }
  delay(100);
}

void SendMessage() {
  mySerial2.println("AT+CMGF=1");
  delay(1000);
  mySerial2.println("AT+CMGS=\"+919345516234\"\r");
  delay(1000);
  mySerial2.println("Water has been detected less than 3 feet from top");
  delay(100);
  mySerial2.println((char)26);
  delay(1000);
}

void SendMessage1() {
  mySerial2.println("AT+CMGF=1");
  delay(1000);
  mySerial2.println("AT+CMGS=\"+919345516234\"\r");
  delay(1000);
  mySerial2.println("Water has been detected less than 5 feet from top");
  delay(100);
  mySerial2.println((char)26);
  delay(1000);
}

void SendMessage2() {
  mySerial2.println("AT+CMGF=1");
  delay(1000);
  mySerial2.println("AT+CMGS=\"+919345516234\"\r");
  delay(1000);
  mySerial2.println("Water has been detected less than 7 feet from top");
  delay(100);
  mySerial2.println((char)26);
  delay(1000);
}

void MakeCall() {
  mySerial2.println("ATD+919345516234;");
  Serial.println("Calling ");
  delay(10000); 
}

void CheckCallStatus() {
  mySerial2.println("AT+CLCC");
  delay(1000);
  while (mySerial2.available()) {
    Serial.write(mySerial2.read());
  }
}
