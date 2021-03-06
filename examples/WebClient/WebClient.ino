/**************************************************************
 *
 * This sketch connects to a website and downloads a page.
 * It can be used to perform HTTP/RESTful API calls.
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 **************************************************************/

#include <TinyGsmClient.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

// Use Hardware Serial on Mega, Leonardo, Micro
#define GsmSerial Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial GsmSerial(2, 3); // RX, TX

TinyGsmClient client(GsmSerial);

char server[] = "cdn.rawgit.com";
char resource[] = "/vshymanskyy/tinygsm/master/extras/logo.txt";

void setup() {
  // Set console baud rate
  Serial.begin(115200);
  delay(10);

  // Set GSM module baud rate
  GsmSerial.begin(115200);
  delay(3000);

  // Restart takes quite some time
  // You can skip it in many cases
  Serial.println("Restarting modem...");
  client.restart();
}

void loop() {
  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!client.networkConnect(apn, user, pass)) {
    Serial.println(" failed");
    delay(10000);
    return;
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(server);
  if (!client.connect(server, 80)) {
    Serial.println(" failed");
    delay(10000);
    return;
  }
  Serial.println(" OK");

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
    }
  }
  Serial.println();

  client.stop();
  Serial.println("Server disconnected");

  client.networkDisconnect();
  Serial.println("GPRS disconnected");

  // Do nothing forevermore
  while (true) {
    delay(1000);
  }
}

