

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = SECRET_WIFI1_SSID;
const char* password = SECRET_WIFI1_PASS;

const char mdns_hostname[] = SECRET_HOST;

const char* mqtt_server = SECRET_MQTT_SERVER;
const int mqtt_port = 1883;

const char* mqtt_listen = SECRET_MQTT_LISTEN;
const char* mqtt_registration = SECRET_MQTT_REGISTRATION;
const char* mqtt_debug = "/debug";

const char* MQTT_USER = SECRET_MQTT_USER;
const char* MQTT_PASSWORD = SECRET_MQTT_PASS;

const String localTimezoneString = SECRET_TIMEZONE_POSIX;

// If you want to have a different date or clock format change these two
// Complete table with every char: https://github.com/ropg/ezTime#getting-date-and-time
String dateFormat = "d.m.Y"; //Examples: d.m.Y -> 11.09.2021, D M y -> SAT SEP 21
String clockFormat = "H:i"; // Examples: H:i -> 21:19, h:ia -> 09:19PM

const char letters[] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'Ä', 'Ö', 'Ü', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', '.', '-', '?', '!'};
int displayState[UNITSAMOUNT];
String writtenLast;
String lastAlignment;
unsigned long previousMillis = 0;

// Search for parameter in HTTP POST request
const char* PARAM_ALIGNMENT = "alignment";
const char* PARAM_SPEED = "flapspeed";
const char* PARAM_DEVICEMODE = "devicemode";
const char* PARAM_TEXT = "flaptext";


// File paths to save input values permanently
const char* alignmentPath = "/alignment.txt";
const char* flapspeedPath = "/flapspeed.txt";
const char* devicemodePath = "/devicemode.txt";
const char* flaptextPath = "/flaptext.txt";

JSONVar values;

char ip_char[16];
String reg;

static int restartNow = false;
