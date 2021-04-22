/** SYSTEM **/

int chipID;
char SSID[30];
char in_topic[25];
char out_topic[25];
char open_time_char[3];
char close_time_char[3];
char HOSTNAME[50];
const int led = 13;


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

void system_setup() {
  create_ssid();
  create_in_topic();
  create_out_topic();
  create_hostname();
  String time = "";
  time += open_time;
  time.toCharArray(open_time_char, 3);
  time = "";
  time += close_time;
  time.toCharArray(close_time_char, 3);
}