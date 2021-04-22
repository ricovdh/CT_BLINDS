/** UPDATE **/

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
  client.publish(out_topic, "status,Update started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
  client.publish(out_topic, "status,Update finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
  char msg[50];
  sprintf(msg, "status,Update at %d of %d bytes", cur, total);
  client.publish(out_topic, msg);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
  char msg[50];
  sprintf(msg, "status,Update fail. Error: %d", err);
  client.publish(out_topic, msg);
}

void update() {
  // wait for WiFi connection
    Serial.println("update started");
    WiFiClient wificlient;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);

    t_httpUpdate_return ret = ESPhttpUpdate.update(wificlient, "http://clevrthings.com:1880/update", VERSION);
    // Or:
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        clear_message();
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        client.publish(out_topic, "status,No update available");
        clear_message();
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        client.publish(out_topic, "status,Update OK");
        clear_message();
        break;
    }
}