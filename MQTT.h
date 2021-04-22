/** MQTT **/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

int int_message = 200;
bool check_incoming_message = false;

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
  payload[length] = '\0'; // Add a NULL to the end of the char* to make it a string.
  int_message = atoi((char *)payload);
  Serial.println();
  check_incoming_message = true;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(HOSTNAME, MQTT_USER,MQTT_PASS)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sprintf(position,"%ld", get_cur_position());
      client.publish(out_topic, VERSION);
      client.publish(out_topic, position);
      // ... and resubscribe
      client.subscribe(in_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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





// #include <ESP8266MQTTClient.h>
// MQTTClient mqtt;

// void mqtt_setup() {

//   //topic, data, data is continuing
//   mqtt.onData([](String topic, String data, bool cont) {
//     int_message = data.toInt();
//     check_incoming_message = true;
//   });

//   mqtt.onSubscribe([](int sub_id) {
//     char position[3];
//     sprintf(position,"%ld", get_cur_position());
//     mqtt.publish(out_topic, VERSION, 0, 0);
//     mqtt.publish(out_topic, position);
//   });

//   mqtt.onConnect([]() {
//   });

//   mqtt.onDisconnect([]() {
//   });

//   String mqtt_address = "mqtt://";
//   mqtt_address += MQTT_USER;
//   mqtt_address += ":";
//   mqtt_address += MQTT_PASS;
//   mqtt_address += "@";
//   mqtt_address += MQTT_SERVER;
//   mqtt_address += ":";
//   mqtt_address += MQTT_PORT;

//   mqtt.begin(mqtt_address, 60, true);
//   //mqtt.begin(mqtt_address, {.lwtTopic = out_topic, .lwtMsg = "offline", .lwtQos = 0, .lwtRetain = 0});

//   mqtt.subscribe(in_topic, 1);


// }