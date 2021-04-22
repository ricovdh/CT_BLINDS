/** SETTINGS **/

#define VERSION "0.1.0"

#define SHORT_NAME "BLINDS" // the device name
#define AP_PASS "CLEVRthings" // password for AP mode

#define MQTT_SERVER "clevrthings-hub.local"
#define MQTT_PORT 1883
#define MQTT_USER "clevrthings"
#define MQTT_PASS "CTsystemMQTT0509"
#define MQTT_IN_TOPIC "IN"
#define MQTT_OUT_TOPIC "OUT"

#define BAUDRATE 9600

#define DEFAULT_BLINDS_TRAVEL_TIME 10 // seconds for the blinds to open or close (complete travel)
#define first_boot_address 0
#define open_time_address 1
#define close_time_address 2
#define cur_position_address 3

#define LONG_PRESS_TIME 3000

/** MQTT messages:
0 - 100 = position
101     = update
102     = config
103     = reboot
200     = clear message
**/

// INPUTS & OUTPUTS
const int switch_down = 9; // input
const int switch_up = 0; // input
const int button = 10; // small button input
const int open = 5; // open relay output
const int close = 12; // close relay output
const int led = 13; // output


/** VARIABLES **/

// SYSTEM
int chipID;


// MEM
#include <EEPROM.h>
byte first_boot;


// WIFI
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter custom_open_time("opentime", "Blinds open sec", "", 3);
WiFiManagerParameter custom_close_time("closetime", "Blinds close sec", "", 3);
std::vector<const char *> menu = { "wifi", "sep", "erase", "restart", "exit" };
bool wifi_connected;
char SSID[30];
char HOSTNAME[50];


// MQTT
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);
int int_message = 200; // 200 is not used so use as default
char in_topic[25]; // buffer to store the generated topic
char out_topic[25];
bool check_incoming_message = false;


// UPDATE
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>


// OTA
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
ESP8266WebServer server(80);
const char *serverIndex =
  "<form method='POST' action='/update' enctype='multipart/form-data'><input "
  "type='file' name='update'><input type='submit' value='Update'></form>";
bool ota_setup_flag = false;
bool ota_run = false;


// CONTROL
long buttonTimer = 0;
bool buttonActive = false;
bool longPressActive = false;
int switch_down_last_state = 1; // 1 = off
int switch_up_last_state = 1; // 1 = off
bool open_active = false;
bool close_active = false;
byte open_time;
byte close_time;
char open_time_char[3];
char close_time_char[3];
unsigned long moving_start_time = 0;
long max_travel = 0;
bool manual_override = false;
int current_position;
char position[15];




