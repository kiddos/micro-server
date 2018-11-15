#include <ESP8266WiFi.h>
#include <SD.h>

#include "http_parser.h"
#include "json_processor.h"

#ifndef TARGET_SSID
#define TARGET_SSID "SSID"
#endif

#ifndef PASSWORD
#define PASSWORD "12345678"
#endif

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(TARGET_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(TARGET_SSID, PASSWORD);

  if (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("server started");

  Serial.println(WiFi.localIP());

  // if (!SD.begin(9)) {
  //   Serial.println("SD card module initialization failed!");
  // } else {
  //   Serial.println("SD card module initialization done.");
  // }

  digitalWrite(13, LOW);

  Serial.println("waiting for clinets...");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("accept client");
  Serial.println();
  while (!client.available()) {
    delay(1);
  }

  String request;
  while (client.available()) {
    request += static_cast<char>(client.read());
  }
  Serial.println(request);

  nerd::HTTPParser parser(request);
  parser.ParseHeader();
  parser.ParseBody();
  Serial.print("method: ");
  Serial.println(parser.method());
  Serial.print("url: ");
  Serial.println(parser.url());

  nerd::JsonProcessor processor(parser.pairs());

  // File output_file = SD.open("test.json");
  // if (output_file) {
  //   String data = processor.Output();
  //   Serial.println(data);
  //   output_file.write(data.c_str(), data.length());
  //   output_file.close();
  // } else {
  //   Serial.println("error opening test.json");
  // }

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nhello world\n";
  client.print(s);
  Serial.println("client disconnected");
}
