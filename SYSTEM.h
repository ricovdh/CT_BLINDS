/** SYSTEM **/

void reboot() {
  ESP.restart();
}

void create_ssid() {
  chipID = ESP.getChipId();
  String ssid = "ClevrThings ";
  ssid += SHORT_NAME;
  ssid += " ";
  ssid += chipID;
  ssid.toCharArray(SSID, 30);
}

void create_in_topic() {
  String itpc = "CT/";
  itpc += SHORT_NAME;
  itpc += "/";
  itpc += chipID;
  itpc += "/";
  itpc += MQTT_IN_TOPIC;
  itpc.toCharArray(in_topic, 25);
}

void create_out_topic() {
  String otpc = "CT/";
  otpc += SHORT_NAME;
  otpc += "/";
  otpc += chipID;
  otpc += "/";
  otpc += MQTT_OUT_TOPIC;
  otpc.toCharArray(out_topic, 25);
}

void create_hostname() {
  String _hostname = "CT-";
  _hostname += SHORT_NAME;
  _hostname += "-";
  _hostname += chipID;
  _hostname.toCharArray(HOSTNAME, 50);
}

void store_open_close_time_to_char() {
  String time = "";
  time += open_time;
  time.toCharArray(open_time_char, 3);
  time = "";
  time += close_time;
  time.toCharArray(close_time_char, 3);
}

void set_pinModes() {
  pinMode(open, OUTPUT);
  pinMode(close, OUTPUT);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(switch_down, INPUT);
  pinMode(switch_up, INPUT);
  digitalWrite(led, LOW);
  digitalWrite(switch_down, LOW);
  digitalWrite(switch_up, LOW);
}

void system_setup() {
  Serial.begin(BAUDRATE);
  create_ssid();
  create_in_topic();
  create_out_topic();
  create_hostname();
  store_open_close_time_to_char();
  set_pinModes();
}