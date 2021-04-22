/*
  To upload through terminal you can use: curl -F "image=@firmware.bin"
  esp8266-webupdate.local/update
*/

bool ota_setup_flag = false;

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
const char *serverIndex =
  "<form method='POST' action='/update' enctype='multipart/form-data'><input "
  "type='file' name='update'><input type='submit' value='Update'></form>";

void ota_setup() {
  MDNS.begin(HOSTNAME);

  if (1) {
    server.on("/reboot", []() {
      server.send(200, "text/plain", "Rebooting");
      delay(1000);
      reboot();
    });
    server.on("/ota", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on(
      "/update", HTTP_POST,
      []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        reboot();
      },
      []() {
        HTTPUpload &upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
          Serial.setDebugOutput(true);
          WiFiUDP::stopAll();
          Serial.printf("Update: %s\n", upload.filename.c_str());
          uint32_t maxSketchSpace =
            (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace)) {  // start with max available
                                                // size
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_END) {
          if (Update.end(
                true)) {  // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n",
                          upload.totalSize);
          } else {
            Update.printError(Serial);
          }
          Serial.setDebugOutput(false);
        }
        yield();
      });

      server.begin();
      MDNS.addService("http", "tcp", 80);
      Serial.printf("Ready! Open http://%s.local in your browser\n", HOSTNAME);
      
      ota_setup_flag = false;
  }
}

void ota_loop(void) {
  server.handleClient();
  MDNS.update();
}