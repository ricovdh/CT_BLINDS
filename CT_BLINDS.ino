/** ClevrThings BLINDS v0.1.0 **/

#include "SETTINGS.h"
#include "MEM.h"
#include "SYSTEM.h"
#include "MQTT.h"
#include "UPDATE.h"
#include "WIFI.h"
#include "OTA.h"
#include "CONTROL.h"

void setup() {
  mem_setup();
  system_setup();
  wifi_setup();
  mqtt_setup();
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
