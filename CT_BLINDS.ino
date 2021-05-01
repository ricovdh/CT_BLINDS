/** ClevrThings BLINDS v0.1.2 **/

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
  if (config_ap_flag) {
    config_ap_flag = false;
    config_ap();
  }
  wifi_loop();
  mqtt_loop();
  incoming_message();
  check_button();
  check_switch();
  ota_loop();
}
