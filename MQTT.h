/** MQTT **/

void clear_message() {
  int_message = 200;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  payload[length] = '\0';  // Add a NULL to the end of the char* to make it a string.
  int_message = atoi((char*)payload);
  Serial.println();
  check_incoming_message = true;
}

void reconnect() {
  if (try_count == 10) {
    config_ap_flag = true;
  }

  // Loop until we're reconnected
  if (!client.connected() && (millis() - reconnect_timer) >= 15000) {
    reconnect_timer = millis();
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      char cur_version[15];
      sprintf(cur_version, "version,%s", VERSION);
      client.publish(out_topic, cur_version);
      sprintf(position, "position,%ld", get_cur_position());
      client.publish(out_topic, position);
      char open_st[15];
      sprintf(open_st, "set_open_time,%s", open_time_char);
      client.publish(out_topic, open_st);
      char close_st[15];
      sprintf(close_st, "set_close_time,%s", close_time_char);
      client.publish(out_topic, close_st);
      client.publish(out_topic, "status,Started");
      // ... and resubscribe
      client.subscribe(in_topic);
      reconnect_timer = 0;
    } else {
      ++try_count;
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void mqtt_setup() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void mqtt_loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}