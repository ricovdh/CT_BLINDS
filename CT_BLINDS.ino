/** ClevrThings - BLINDS **/

#define ATOMIC_FS_UPDATE


#include "SETTINGS.h"
#include "MEM.h"
#include "SYSTEM.h"
#include "MQTT.h"
#include "UPDATE.h"
#include "WIFI.h"
#include "OTA.h"
#include "CONTROL.h"

void setup() {
  Serial.begin(9600);
  mem_setup();
  system_setup();
  wifi_setup();
  mqtt_setup();
  pinMode(open, OUTPUT);
  pinMode(close, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(button, INPUT);
  pinMode(switch_down, INPUT);
  pinMode(switch_up, INPUT);
  digitalWrite(switch_down, LOW);
  digitalWrite(switch_up, LOW);
  ota_setup();
}

void loop() {
  wifi_loop();
  mqtt_loop();
  incoming_message();
  check_button();
  check_switch();
  delay(100);
  ota_loop();
}
