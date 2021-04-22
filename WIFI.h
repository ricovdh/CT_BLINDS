/** WIFI **/

#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter custom_open_time("opentime", "Blinds open sec", "", 3);
WiFiManagerParameter custom_close_time("closetime", "Blinds close sec", "", 3);
std::vector<const char *> menu = { "wifi", "sep", "erase", "restart", "exit" };
bool wifi_connected;

void saveWifiCallback() {
  Serial.println("[CALLBACK] saveCallback fired");
}

//gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");

  int time;

  time = atoi(custom_open_time.getValue());
  EEPROM.write(open_time_address, time);
  Serial.print("New open time: ");
  Serial.println(time);

  time = atoi(custom_close_time.getValue());
  EEPROM.write(close_time_address, time);
  Serial.print("New close time: ");
  Serial.println(time);

  eeprom_commit();
  
  // reboot();
}

void handleRoute() {
  Serial.println("[HTTP] handle route");
  wm.server->send(200, "text/plain", "hello from user code");
}

void bindServerCallback() {
  wm.server->on("/custom", handleRoute);
  // wm.server->on("/info",handleRoute); // you can override wm!
}

void wifi_setup() {

  WiFi.mode(WIFI_STA);
  //wm.resetSettings();

  wm.setClass("invert");  
  wm.setShowPassword(true);
  wm.setConnectTimeout(20);

  wm.addParameter(&custom_open_time);
  wm.addParameter(&custom_close_time);

  wm.setConfigPortalBlocking(false);
  
  // callbacks
  wm.setAPCallback(configModeCallback);
  wm.setWebServerCallback(bindServerCallback);
  wm.setSaveConfigCallback(saveWifiCallback);
  wm.setSaveParamsCallback(saveParamCallback);

  custom_open_time.setValue(open_time_char,3);
  custom_close_time.setValue(close_time_char,3);

  //std::vector<const char *> menu = { "wifi", "sep", "erase", "restart", "exit" };
  wm.setMenu(menu);  // custom menu, pass vector

  // set Hostname
  wm.setHostname(HOSTNAME);

  // set custom channel
  // wm.setWiFiAPChannel(13);

  wm.setConfigPortalTimeout(180);

  // show static ip fields
  wm.setShowStaticFields(true);

  wifi_connected = wm.autoConnect(SSID, AP_PASS);

  if(wifi_connected){
      Serial.println("connected...yeey :)");
  }
  else {
      Serial.println("Configportal running");
      reboot();
  }

  wm.setBreakAfterConfig(true);
}

void wifi_loop() {
  if (!wifi_connected) {
    reboot();
  }
}

void config_ap() {
    wm.startConfigPortal(SSID, AP_PASS);
}