/** SETTINGS **/

#define VERSION "0.0.2"

#define SHORT_NAME "BLINDS" // the device name
#define AP_PASS "CLEVRthings" // password for AP mode

#define MQTT_SERVER "clevrthings-hub.local"
#define MQTT_PORT 1883
#define MQTT_USER "clevrthings"
#define MQTT_PASS "CTsystemMQTT0509"
#define MQTT_IN_TOPIC "IN"
#define MQTT_OUT_TOPIC "OUT"

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