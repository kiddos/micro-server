#include <ESP8266WiFi.h>
#include <SD.h>

#include "file_manager.h"
#include "http_parser.h"
#include "json_processor.h"

#ifndef TARGET_SSID
#define TARGET_SSID "SSID"
#endif

#ifndef PASSWORD
#define PASSWORD "12345678"
#endif

#ifndef LED_PIN
#define LED_PIN 2
#endif

#ifndef DATA_DIR
#define DATA_DIR "/data"
#endif

WiFiServer server(80);
nerd::FileManager file_manager;

void ListFiles(File dir, int num_tabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    for (uint16_t i = 0; i < num_tabs; ++i) {
      Serial.print('\t');
    }
    Serial.print(entry.name());

    if (entry.isDirectory()) {
      Serial.println('/');
      ListFiles(entry, num_tabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(9600);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(TARGET_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(TARGET_SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("server started");

  Serial.println(WiFi.localIP());

  // initialize sd card
  if (!SD.begin(10)) {
    Serial.println("SD card module initialization failed!");
  } else {
    Serial.println("SD card module initialization done.");
    Serial.println();
    Serial.println("Files: ");
    File root = SD.open("/");
    ListFiles(root, 0);

    // make data directory
    if (!SD.exists(DATA_DIR)) {
      Serial.println("create data directory...");
      SD.mkdir(DATA_DIR);
    }

    // check starting index
    file_manager.set_data_dir(DATA_DIR);
    while (SD.exists(file_manager.GetFileName())) {
      file_manager.Next();
    }
    Serial.print("data starting at ");
    Serial.println(file_manager.index());
  }

  Serial.println("waiting for clients...");

  digitalWrite(LED_PIN, LOW);
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
  // Serial.println(request);

  nerd::HTTPParser parser(request);
  parser.ParseHeader();
  parser.ParseBody();

  Serial.print("method: ");
  Serial.println(parser.method());
  Serial.print("url: ");
  Serial.println(parser.url());

  if (parser.method() == "POST") {
    nerd::JsonProcessor processor(parser.pairs());

    String filename = file_manager.NextFileName();

    // post to different url directory
    if (parser.url() != "/") {
      nerd::FileManager url_manager;
      url_manager.set_data_dir(parser.url());
      while (SD.exists(url_manager.GetFileName())) {
        url_manager.Next();
      }
      filename = url_manager.NextFileName();
    }

    // write to file
    File output_file = SD.open(filename, FILE_WRITE);
    bool success = false;
    if (output_file) {
      Serial.print("LOG: writing data to ");
      Serial.println(filename);

      String data = processor.Output();
      output_file.write(data.c_str(), data.length());
      output_file.close();
      success = true;
    } else {
      Serial.print("ERROR: fail to open ");
      Serial.println(filename);
    }

    client.print(
        F("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"));
    if (success) {
      client.print("{\"status\": 1}");
    } else {
      client.print("{\"status\": 0}");
    }
  } else if (parser.method() == "GET") {
    client.print(
        F("HTTP/1.1 200 OK\r\n"
          "Content-Type: application/json\r\n\r\n"));
    client.print("{\n");
    client.print("\"files\": " + String(file_manager.index()) + ",\n");
    client.print("\"next\": " + file_manager.GetFileName() + "\n");
    client.print("}");
  } else {
    client.print(
        F("HTTP/1.1 404 Not Found\r\n"
          "Content-Type: text/html\r\n"
          "Connection: Closed\r\n\r\n"
          "<!DOCTYPE html>\n"
          "<html>\n"
          "  <head>\n"
          "    <title>404 Not Found</title>\n"
          "  </head>\n"
          "  <body>\n"
          "    <h1>Not Found</h1>\n"
          "  </body>\n"
          "</html>\n"));
  }

  Serial.println("client disconnected");
}
