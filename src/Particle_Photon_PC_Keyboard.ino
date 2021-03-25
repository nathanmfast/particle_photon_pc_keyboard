/*
 * Project: particle_photon_pc_keyboard
 * Description: Particle Photon firmware to use the device as a USB keyboard with MQTT support.
 * Author: Nathan Fast
 * Date: March 13, 2021
 */

/*
Enabling System Thread will cause setup() and loop() to run as normal but without WiFi or Particle Cloud connectivity having to be established first.
A call to WiFi.connect() will still block your application thread, but a call to Particle.connect() will not. 

Particle Documentation:
https://docs.particle.io/reference/device-os/firmware/photon/#system-thread
*/
SYSTEM_THREAD(ENABLED);

/*
System Modes:
- SYSTEM_MODE(MANUAL);
  - Requires you to establish a connection to Particle Cloud and handle communications by calling Particle.process() on a regular basis.
- SYSTEM_MODE(SEMI_AUTOMATIC);
  - Requires you to establish a connection to Particle Cloud, but once connected handles communications automatically.
- SYSTEM_MODE(AUTOMATIC);
  - Connectes to Particle Cloud and handles communication automatically.

I'm using SEMI_AUTOMATIC for this so I can get MQTT up and running before trying to connect to Particle Cloud.

For some reason this also worked using MANUAL mode without explicit calls to Particle.process(), but I suspect this is a bug.

Particle Documentation:
https://docs.particle.io/reference/device-os/firmware/photon/#system-modes
*/
SYSTEM_MODE(SEMI_AUTOMATIC);

/*
Log Levels:
- LOG_LEVEL_ALL (special value that can be used to enable logging of all messages)
- LOG_LEVEL_TRACE (verbose output for debugging purposes)
- LOG_LEVEL_INFO (regular information messages)
- LOG_LEVEL_WARN (warnings and non-critical errors)
- LOG_LEVEL_ERROR (error messages)
- LOG_LEVEL_NONE (special value that can be used to disable logging of any messages)

Particle Documentation:
https://docs.particle.io/reference/device-os/firmware/photon/#logging-levels
*/
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#include "config.h"
#include <string>
#include <map>
#include "MQTT.h"

#pragma region MQTT Setup

// buffer used to send/receive data with MQTT

const uint8_t MSG_BUFFER_SIZE = 20;
char m_msg_buffer[MSG_BUFFER_SIZE]; 

// callback stub / client

void callback(char* p_topic, uint8_t* p_payload, unsigned int p_length);

MQTT client(((const char *)CONFIG_MQTT_HOST), CONFIG_MQTT_PORT, callback); 

#pragma endregion

#pragma region Keys

//TODO: Sort out a better way to handle this because this mapping object in its entirety won't even fit in the memory space of the Photon.
std::map<String, UsbKeyboardScanCode> key_map = {
    {"KEY_RESERVED", KEY_RESERVED},
    {"KEY_A", KEY_A},
    {"KEY_B", KEY_B},
    {"KEY_C", KEY_C},
    {"KEY_D", KEY_D},
    {"KEY_E", KEY_E},
    {"KEY_F", KEY_F},
    {"KEY_G", KEY_G},
    {"KEY_H", KEY_H},
    {"KEY_I", KEY_I},
    {"KEY_J", KEY_J},
    {"KEY_K", KEY_K},
    {"KEY_L", KEY_L},
    {"KEY_M", KEY_M},
    {"KEY_N", KEY_N},
    {"KEY_O", KEY_O},
    {"KEY_P", KEY_P},
    {"KEY_Q", KEY_Q},
    {"KEY_R", KEY_R},
    {"KEY_S", KEY_S},
    {"KEY_T", KEY_T},
    {"KEY_U", KEY_U},
    {"KEY_V", KEY_V},
    {"KEY_W", KEY_W},
    {"KEY_X", KEY_X},
    {"KEY_Y", KEY_Y},
    {"KEY_Z", KEY_Z},
    {"KEY_1", KEY_1},
    {"KEY_2", KEY_2},
    {"KEY_3", KEY_3},
    {"KEY_4", KEY_4},
    {"KEY_5", KEY_5},
    {"KEY_6", KEY_6},
    {"KEY_7", KEY_7},
    {"KEY_8", KEY_8},
    {"KEY_9", KEY_9},
    {"KEY_0", KEY_0},
    {"KEY_ENTER", KEY_ENTER},
    {"KEY_ESC", KEY_ESC},
    {"KEY_BACKSPACE", KEY_BACKSPACE},
    {"KEY_TAB", KEY_TAB},
    {"KEY_SPACE", KEY_SPACE},
    {"KEY_MINUS", KEY_MINUS},
    {"KEY_EQUAL", KEY_EQUAL},
    {"KEY_LEFTBRACE", KEY_LEFTBRACE},
    {"KEY_RIGHTBRACE", KEY_RIGHTBRACE},
    {"KEY_BACKSLASH", KEY_BACKSLASH},
    {"KEY_HASH", KEY_HASH},
    {"KEY_SEMICOLON", KEY_SEMICOLON},
    {"KEY_APOSTROPHE", KEY_APOSTROPHE},
    {"KEY_GRAVE", KEY_GRAVE},
    {"KEY_COMMA", KEY_COMMA},
    {"KEY_DOT", KEY_DOT},
    {"KEY_SLASH", KEY_SLASH},
    {"KEY_CAPSLOCK", KEY_CAPSLOCK},
    {"KEY_F1", KEY_F1},
    {"KEY_F2", KEY_F2},
    {"KEY_F3", KEY_F3},
    {"KEY_F4", KEY_F4},
    {"KEY_F5", KEY_F5},
    {"KEY_F6", KEY_F6},
    {"KEY_F7", KEY_F7},
    {"KEY_F8", KEY_F8},
    {"KEY_F9", KEY_F9},
    {"KEY_F10", KEY_F10},
    {"KEY_F11", KEY_F11},
    {"KEY_F12", KEY_F12},
    {"KEY_PRINT", KEY_PRINT},
    {"KEY_SCROLLLOCK", KEY_SCROLLLOCK},
    {"KEY_PAUSE", KEY_PAUSE},
    {"KEY_INSERT", KEY_INSERT},
    {"KEY_HOME", KEY_HOME},
    {"KEY_PAGEUP", KEY_PAGEUP},
    {"KEY_DELETE", KEY_DELETE},
    {"KEY_END", KEY_END},
    {"KEY_PAGEDOWN", KEY_PAGEDOWN},
    {"KEY_RIGHT", KEY_RIGHT},
    {"KEY_LEFT", KEY_LEFT},
    {"KEY_DOWN", KEY_DOWN},
    {"KEY_UP", KEY_UP},
    {"KEY_NUMLOCK", KEY_NUMLOCK},
    {"KEY_KPSLASH", KEY_KPSLASH},
    {"KEY_KPASTERISK", KEY_KPASTERISK},
    {"KEY_KPMINUS", KEY_KPMINUS},
    {"KEY_KPPLUS", KEY_KPPLUS},
    {"KEY_KPENTER", KEY_KPENTER},
    {"KEY_KP1", KEY_KP1},
    {"KEY_KP2", KEY_KP2},
    {"KEY_KP3", KEY_KP3},
    {"KEY_KP4", KEY_KP4},
    {"KEY_KP5", KEY_KP5},
    {"KEY_KP6", KEY_KP6},
    {"KEY_KP7", KEY_KP7},
    {"KEY_KP8", KEY_KP8},
    {"KEY_KP9", KEY_KP9},
    {"KEY_KP0", KEY_KP0},
    {"KEY_KPDOT", KEY_KPDOT},
    {"KEY_102ND", KEY_102ND},
    {"KEY_COMPOSE", KEY_COMPOSE},
    {"KEY_POWER", KEY_POWER},
    {"KEY_KPEQUAL", KEY_KPEQUAL},
    {"KEY_F13", KEY_F13},
    {"KEY_F14", KEY_F14},
    {"KEY_F15", KEY_F15},
    {"KEY_F16", KEY_F16},
    {"KEY_F17", KEY_F17},
    {"KEY_F18", KEY_F18},
    {"KEY_F19", KEY_F19},
    {"KEY_F20", KEY_F20},
    {"KEY_F21", KEY_F21},
    {"KEY_F22", KEY_F22},
    {"KEY_F23", KEY_F23},
    {"KEY_F24", KEY_F24},
    {"KEY_OPEN", KEY_OPEN},
    {"KEY_HELP", KEY_HELP},
    {"KEY_MENU", KEY_MENU},
    {"KEY_SELECT", KEY_SELECT},
    {"KEY_STOP", KEY_STOP},
    {"KEY_AGAIN", KEY_AGAIN},
    {"KEY_UNDO", KEY_UNDO},
    {"KEY_CUT", KEY_CUT},
    {"KEY_COPY", KEY_COPY},
    {"KEY_PASTE", KEY_PASTE},
    {"KEY_FIND", KEY_FIND},
    {"KEY_MUTE", KEY_MUTE},
    {"KEY_VOLUMEUP", KEY_VOLUMEUP},
    {"KEY_VOLUMEDOWN", KEY_VOLUMEDOWN},
    {"KEY_LOCKCAPSLOCK", KEY_LOCKCAPSLOCK},
    {"KEY_LOCKNUMLOCK", KEY_LOCKNUMLOCK},
    {"KEY_LOCKSCROLLOCK", KEY_LOCKSCROLLOCK}
    // {"KEY_KPCOMMA", KEY_KPCOMMA},
    // {"KEY_KPEQUALSIGN", KEY_KPEQUALSIGN},
    // {"KEY_RO", KEY_RO},
    // {"KEY_KATAKANAHIRAGANA", KEY_KATAKANAHIRAGANA},
    // {"KEY_YEN", KEY_YEN},
    // {"KEY_HENKAN", KEY_HENKAN},
    // {"KEY_MUHENKAN", KEY_MUHENKAN},
    // {"KEY_KPJPCOMMA", KEY_KPJPCOMMA},
    // {"KEY_INTL7", KEY_INTL7},
    // {"KEY_INTL8", KEY_INTL8},
    // {"KEY_INTL9", KEY_INTL9},
    // {"KEY_HANGEUL", KEY_HANGEUL},
    // {"KEY_HANJA", KEY_HANJA},
    // {"KEY_KATAKANA", KEY_KATAKANA},
    // {"KEY_HIRAGANA", KEY_HIRAGANA},
    // {"KEY_ZENKAKUHENKAKU", KEY_ZENKAKUHENKAKU},
    // {"KEY_LANG6", KEY_LANG6},
    // {"KEY_LANG7", KEY_LANG7},
    // {"KEY_LANG8", KEY_LANG8},
    // {"KEY_LANG9", KEY_LANG9},
    // {"KEY_ALTERASE", KEY_ALTERASE},
    // {"KEY_SYSRQ", KEY_SYSRQ},
    // {"KEY_CANCEL", KEY_CANCEL},
    // {"KEY_CLEAR", KEY_CLEAR},
    // {"KEY_PRIOR", KEY_PRIOR},
    // {"KEY_ENTER2", KEY_ENTER2},
    // {"KEY_SEPARATOR", KEY_SEPARATOR},
    // {"KEY_OUT", KEY_OUT},
    // {"KEY_OPER", KEY_OPER},
    // {"KEY_CLRAGAIN", KEY_CLRAGAIN},
    // {"KEY_CRSEL", KEY_CRSEL},
    // {"KEY_EXSEL", KEY_EXSEL},
    // {"KEY_KP00", KEY_KP00},
    // {"KEY_KP000", KEY_KP000},
    // {"KEY_THOUSANDSEP", KEY_THOUSANDSEP},
    // {"KEY_DECIMALSEP", KEY_DECIMALSEP},
    // {"KEY_CURRENCY", KEY_CURRENCY},
    // {"KEY_CURRENCYSUB", KEY_CURRENCYSUB},
    // {"KEY_KPLEFTPAREN", KEY_KPLEFTPAREN},
    // {"KEY_KPRIGHTPAREN", KEY_KPRIGHTPAREN},
    // {"KEY_KPLEFTBRACE", KEY_KPLEFTBRACE},
    // {"KEY_KPRIGHTBRACE", KEY_KPRIGHTBRACE},
    // {"KEY_KPTAB", KEY_KPTAB},
    // {"KEY_KPBACKSPACE", KEY_KPBACKSPACE},
    // {"KEY_KPA", KEY_KPA},
    // {"KEY_KPB", KEY_KPB},
    // {"KEY_KPC", KEY_KPC},
    // {"KEY_KPD", KEY_KPD},
    // {"KEY_KPE", KEY_KPE},
    // {"KEY_KPF", KEY_KPF},
    // {"KEY_KPXOR", KEY_KPXOR},
    // {"KEY_KPPOWER", KEY_KPPOWER},
    // {"KEY_KPPERCENT", KEY_KPPERCENT},
    // {"KEY_KPLT", KEY_KPLT},
    // {"KEY_KPGT", KEY_KPGT},
    // {"KEY_KPAMP", KEY_KPAMP},
    // {"KEY_KPAMPAMP", KEY_KPAMPAMP},
    // {"KEY_KPBAR", KEY_KPBAR},
    // {"KEY_KPBARBAR", KEY_KPBARBAR},
    // {"KEY_KPCOLON", KEY_KPCOLON},
    // {"KEY_KPHASH", KEY_KPHASH},
    // {"KEY_KPSPACE", KEY_KPSPACE},
    // {"KEY_KPAT", KEY_KPAT},
    // {"KEY_KPEXCLAM", KEY_KPEXCLAM},
    // {"KEY_KPMEMSTORE", KEY_KPMEMSTORE},
    // {"KEY_KPMEMRECALL", KEY_KPMEMRECALL},
    // {"KEY_KPMEMCLEAR", KEY_KPMEMCLEAR},
    // {"KEY_KPMEMADD", KEY_KPMEMADD},
    // {"KEY_KPMEMSUB", KEY_KPMEMSUB},
    // {"KEY_KPMEMMULT", KEY_KPMEMMULT},
    // {"KEY_KPMEMDIV", KEY_KPMEMDIV},
    // {"KEY_KPPLUSMINUS", KEY_KPPLUSMINUS},
    // {"KEY_KPCLEAR", KEY_KPCLEAR},
    // {"KEY_KPCLEARENT", KEY_KPCLEARENT},
    // {"KEY_KPBINARY", KEY_KPBINARY},
    // {"KEY_KPOCTAL", KEY_KPOCTAL},
    // {"KEY_KPDECIMAL", KEY_KPDECIMAL},
    // {"KEY_KPHEX", KEY_KPHEX},
    // {"KEY_LEFTCTRL", KEY_LEFTCTRL},
    // {"KEY_LEFTSHIFT", KEY_LEFTSHIFT},
    // {"KEY_LEFTALT", KEY_LEFTALT},
    // {"KEY_LEFTGUI", KEY_LEFTGUI},
    // {"KEY_RIGHTCTRL", KEY_RIGHTCTRL},
    // {"KEY_RIGHTSHIFT", KEY_RIGHTSHIFT},
    // {"KEY_RIGHTALT", KEY_RIGHTALT},
    // {"KEY_RIGHTGUI", KEY_RIGHTGUI},
    // {"KEY_LEFT_WINDOWS", KEY_LEFT_WINDOWS},
    // {"KEY_RIGHT_WINDOWS", KEY_RIGHT_WINDOWS},
    // {"KEY_LEFT_COMMAND", KEY_LEFT_COMMAND},
    // {"KEY_RIGHT_COMMAND", KEY_RIGHT_COMMAND},
    // {"KEY_LEFT_META", KEY_LEFT_META},
    // {"KEY_RIGHT_META", KEY_RIGHT_META},
    // {"KEY_INTL1", KEY_INTL1},
    // {"KEY_INTL2", KEY_INTL2},
    // {"KEY_INTL3", KEY_INTL3},
    // {"KEY_INTL4", KEY_INTL4},
    // {"KEY_INTL5", KEY_INTL5},
    // {"KEY_INTL6", KEY_INTL6},
    // {"KEY_LANG1", KEY_LANG1},
    // {"KEY_LANG2", KEY_LANG2},
    // {"KEY_LANG3", KEY_LANG3},
    // {"KEY_LANG4", KEY_LANG4},
    // {"KEY_LANG5", KEY_LANG5},
    // {"KEY_UNKNOWN", KEY_UNKNOWN},
    // {"KEY_ERROR_ROLLOVER", KEY_ERROR_ROLLOVER},
    // {"KEY_POSTFAIL", KEY_POSTFAIL},
    // {"KEY_ERROR_UNDEFINED", KEY_ERROR_UNDEFINED},
    // {"KEY_RETURN", KEY_RETURN},
    // {"KEY_ESCAPE", KEY_ESCAPE},
    // {"KEY_EQUALS", KEY_EQUALS},
    // {"KEY_LEFTBRACKET", KEY_LEFTBRACKET},
    // {"KEY_RIGHTBRACKET", KEY_RIGHTBRACKET},
    // {"KEY_NONUSHASH", KEY_NONUSHASH},
    // {"KEY_PERIOD", KEY_PERIOD},
    // {"KEY_PRINTSCREEN", KEY_PRINTSCREEN},
    // {"KEY_NUMLOCKCLEAR", KEY_NUMLOCKCLEAR},
    // {"KEY_KP_DIVIDE", KEY_KP_DIVIDE},
    // {"KEY_KP_MULTIPLY", KEY_KP_MULTIPLY},
    // {"KEY_KP_MINUS", KEY_KP_MINUS},
    // {"KEY_KP_PLUS", KEY_KP_PLUS},
    // {"KEY_KP_ENTER", KEY_KP_ENTER},
    // {"KEY_KP_1", KEY_KP_1},
    // {"KEY_KP_2", KEY_KP_2},
    // {"KEY_KP_3", KEY_KP_3},
    // {"KEY_KP_4", KEY_KP_4},
    // {"KEY_KP_5", KEY_KP_5},
    // {"KEY_KP_6", KEY_KP_6},
    // {"KEY_KP_7", KEY_KP_7},
    // {"KEY_KP_8", KEY_KP_8},
    // {"KEY_KP_9", KEY_KP_9},
    // {"KEY_KP_0", KEY_KP_0},
    // {"KEY_KP_PERIOD", KEY_KP_PERIOD},
    // {"KEY_NONUSBACKSLASH", KEY_NONUSBACKSLASH},
    // {"KEY_APPLICATION", KEY_APPLICATION},
    // {"KEY_KP_EQUALS", KEY_KP_EQUALS},
    // {"KEY_EXECUTE", KEY_EXECUTE},
    // {"KEY_LOCKINGCAPSLOCK", KEY_LOCKINGCAPSLOCK},
    // {"KEY_LOCKINGNUMLOCK", KEY_LOCKINGNUMLOCK},
    // {"KEY_LOCKINGSCROLLOCK", KEY_LOCKINGSCROLLOCK},
    // {"KEY_KP_COMMA", KEY_KP_COMMA},
    // {"KEY_KP_EQUALSAS400", KEY_KP_EQUALSAS400},
    // {"KEY_INTERNATIONAL1", KEY_INTERNATIONAL1},
    // {"KEY_INTERNATIONAL2", KEY_INTERNATIONAL2},
    // {"KEY_INTERNATIONAL3", KEY_INTERNATIONAL3},
    // {"KEY_INTERNATIONAL4", KEY_INTERNATIONAL4},
    // {"KEY_INTERNATIONAL5", KEY_INTERNATIONAL5},
    // {"KEY_INTERNATIONAL6", KEY_INTERNATIONAL6},
    // {"KEY_INTERNATIONAL7", KEY_INTERNATIONAL7},
    // {"KEY_INTERNATIONAL8", KEY_INTERNATIONAL8},
    // {"KEY_INTERNATIONAL9", KEY_INTERNATIONAL9},
    // {"KEY_SYSREQ", KEY_SYSREQ},
    // {"KEY_RETURN2", KEY_RETURN2},
    // {"KEY_CLEARAGAIN", KEY_CLEARAGAIN},
    // {"KEY_KP_00", KEY_KP_00},
    // {"KEY_KP_000", KEY_KP_000},
    // {"KEY_THOUSANDSSEPARATOR", KEY_THOUSANDSSEPARATOR},
    // {"KEY_DECIMALSEPARATOR", KEY_DECIMALSEPARATOR},
    // {"KEY_CURRENCYUNIT", KEY_CURRENCYUNIT},
    // {"KEY_CURRENCYSUBUNIT", KEY_CURRENCYSUBUNIT},
    // {"KEY_KP_LEFTPAREN", KEY_KP_LEFTPAREN},
    // {"KEY_KP_RIGHTPAREN", KEY_KP_RIGHTPAREN},
    // {"KEY_KP_LEFTBRACE", KEY_KP_LEFTBRACE},
    // {"KEY_KP_RIGHTBRACE", KEY_KP_RIGHTBRACE},
    // {"KEY_KP_TAB", KEY_KP_TAB},
    // {"KEY_KP_BACKSPACE", KEY_KP_BACKSPACE},
    // {"KEY_KP_A", KEY_KP_A},
    // {"KEY_KP_B", KEY_KP_B},
    // {"KEY_KP_C", KEY_KP_C},
    // {"KEY_KP_D", KEY_KP_D},
    // {"KEY_KP_E", KEY_KP_E},
    // {"KEY_KP_F", KEY_KP_F},
    // {"KEY_KP_XOR", KEY_KP_XOR},
    // {"KEY_KP_POWER", KEY_KP_POWER},
    // {"KEY_KP_PERCENT", KEY_KP_PERCENT},
    // {"KEY_KP_LESS", KEY_KP_LESS},
    // {"KEY_KP_GREATER", KEY_KP_GREATER},
    // {"KEY_KP_AMPERSAND", KEY_KP_AMPERSAND},
    // {"KEY_KP_DBLAMPERSAND", KEY_KP_DBLAMPERSAND},
    // {"KEY_KP_VERTICALBAR", KEY_KP_VERTICALBAR},
    // {"KEY_KP_DBLVERTICALBAR", KEY_KP_DBLVERTICALBAR},
    // {"KEY_KP_COLON", KEY_KP_COLON},
    // {"KEY_KP_HASH", KEY_KP_HASH},
    // {"KEY_KP_SPACE", KEY_KP_SPACE},
    // {"KEY_KP_AT", KEY_KP_AT},
    // {"KEY_KP_EXCLAM", KEY_KP_EXCLAM},
    // {"KEY_KP_MEMSTORE", KEY_KP_MEMSTORE},
    // {"KEY_KP_MEMRECALL", KEY_KP_MEMRECALL},
    // {"KEY_KP_MEMCLEAR", KEY_KP_MEMCLEAR},
    // {"KEY_KP_MEMADD", KEY_KP_MEMADD},
    // {"KEY_KP_MEMSUBTRACT", KEY_KP_MEMSUBTRACT},
    // {"KEY_KP_MEMMULTIPLY", KEY_KP_MEMMULTIPLY},
    // {"KEY_KP_MEMDIVIDE", KEY_KP_MEMDIVIDE},
    // {"KEY_KP_PLUSMINUS", KEY_KP_PLUSMINUS},
    // {"KEY_KP_CLEAR", KEY_KP_CLEAR},
    // {"KEY_KP_CLEARENTRY", KEY_KP_CLEARENTRY},
    // {"KEY_KP_BINARY", KEY_KP_BINARY},
    // {"KEY_KP_OCTAL", KEY_KP_OCTAL},
    // {"KEY_KP_DECIMAL", KEY_KP_DECIMAL},
    // {"KEY_KP_HEXADECIMAL", KEY_KP_HEXADECIMAL},
    // {"KEY_LCTRL", KEY_LCTRL},
    // {"KEY_LSHIFT", KEY_LSHIFT},
    // {"KEY_LALT", KEY_LALT},
    // {"KEY_LGUI", KEY_LGUI},
    // {"KEY_RCTRL", KEY_RCTRL},
    // {"KEY_RSHIFT", KEY_RSHIFT},
    // {"KEY_RALT", KEY_RALT},
    // {"KEY_RGUI", KEY_RGUI},
    // {"KEY_MODE", KEY_MODE},
    // {"KEY_AUDIONEXT", KEY_AUDIONEXT},
    // {"KEY_AUDIOPREV", KEY_AUDIOPREV},
    // {"KEY_AUDIOSTOP", KEY_AUDIOSTOP},
    // {"KEY_AUDIOPLAY", KEY_AUDIOPLAY},
    // {"KEY_AUDIOMUTE", KEY_AUDIOMUTE},
    // {"KEY_MEDIASELECT", KEY_MEDIASELECT},
    // {"KEY_WWW", KEY_WWW},
    // {"KEY_MAIL", KEY_MAIL},
    // {"KEY_CALCULATOR", KEY_CALCULATOR},
    // {"KEY_COMPUTER", KEY_COMPUTER},
    // {"KEY_AC_SEARCH", KEY_AC_SEARCH},
    // {"KEY_AC_HOME", KEY_AC_HOME},
    // {"KEY_AC_BACK", KEY_AC_BACK},
    // {"KEY_AC_FORWARD", KEY_AC_FORWARD},
    // {"KEY_AC_STOP", KEY_AC_STOP},
    // {"KEY_AC_REFRESH", KEY_AC_REFRESH},
    // {"KEY_AC_BOOKMARKS", KEY_AC_BOOKMARKS},
    // {"KEY_BRIGHTNESSDOWN", KEY_BRIGHTNESSDOWN},
    // {"KEY_BRIGHTNESSUP", KEY_BRIGHTNESSUP},
    // {"KEY_DISPLAYSWITCH", KEY_DISPLAYSWITCH},
    // {"KEY_KBDILLUMTOGGLE", KEY_KBDILLUMTOGGLE},
    // {"KEY_KBDILLUMDOWN", KEY_KBDILLUMDOWN},
    // {"KEY_KBDILLUMUP", KEY_KBDILLUMUP},
    // {"KEY_EJECT", KEY_EJECT},
    // {"KEY_SLEEP", KEY_SLEEP},
    // {"KEY_APP1", KEY_APP1},
    // {"KEY_APP2", KEY_APP2}
};

//TODO: Support modifiers in MQTT / Particle Function capabilities.
std::map<String, UsbKeyboardModifier> mod_map = {
    {"MOD_RESERVED", MOD_RESERVED},
    {"MOD_LEFTCTRL", MOD_LEFTCTRL},
    {"MOD_LEFTSHIFT", MOD_LEFTSHIFT},
    {"MOD_LEFTALT", MOD_LEFTALT},
    {"MOD_LEFTGUI", MOD_LEFTGUI},
    {"MOD_RIGHTCTRL", MOD_RIGHTCTRL},
    {"MOD_RIGHTSHIFT", MOD_RIGHTSHIFT},
    {"MOD_RIGHTALT", MOD_RIGHTALT},
    {"MOD_RIGHTGUI", MOD_RIGHTGUI},
    {"MOD_LCTRL", MOD_LCTRL},
    {"MOD_LSHIFT", MOD_LSHIFT},
    {"MOD_LALT", MOD_LALT},
    {"MOD_LGUI", MOD_LGUI},
    {"MOD_RCTRL", MOD_RCTRL},
    {"MOD_RSHIFT", MOD_RSHIFT},
    {"MOD_RALT", MOD_RALT},
    {"MOD_RGUI", MOD_RGUI},
    {"MOD_LEFT_WINDOWS", MOD_LEFT_WINDOWS},
    {"MOD_RIGHT_WINDOWS", MOD_RIGHT_WINDOWS},
    {"MOD_LEFT_COMMAND", MOD_LEFT_COMMAND},
    {"MOD_RIGHT_COMMAND", MOD_RIGHT_COMMAND},
    {"MOD_LEFT_META", MOD_LEFT_META},
    {"MOD_RIGHT_META", MOD_RIGHT_META}
};

#pragma endregion

#pragma region Device Methods

int login(String pin)
{
    Keyboard.click(KEY_ESC);
    // this delay gives the Login UI a moment to bring up the PIN field
    delay(400);
    Keyboard.write(pin);
    // this extra bit of processing will help cycles through the various options on the login UI, 
    // in case the focus is on the wrong element when this code runs
    for(int i = 1; i <= 4; i++){
        // the lack of delay here means this all happens so fast that even if the pin gets typed in more times 
        // after it is succesfully entered in the correct field, that input doesn't make it into whatever application 
        // you have focused behind the login UI
        Keyboard.click(KEY_ESC);
        Keyboard.click(KEY_TAB);
        Keyboard.write(pin);
    }
    return 0;
}

int tap(String key)
{
    Keyboard.click(key_map[key]);
    return 0;
}

int type(String string)
{
    Keyboard.write(string);
    return 0;
}

#pragma endregion

#pragma region MQTT Methods

#define MQTT_CLIENT_AVAILABILITY_TOPIC CONFIG_MQTT_CLIENT_ID "/availability"

void publishAvailability()
{
    client.publish(MQTT_CLIENT_AVAILABILITY_TOPIC, "online", true);
}

#define MQTT_CLIENT_LOGIN_TOPIC CONFIG_MQTT_CLIENT_ID "/login"
#define MQTT_CLIENT_TAP_TOPIC CONFIG_MQTT_CLIENT_ID "/tap"
#define MQTT_CLIENT_TYPE_TOPIC CONFIG_MQTT_CLIENT_ID "/type"

void subscribe(){
    client.subscribe(MQTT_CLIENT_LOGIN_TOPIC);
    client.subscribe(MQTT_CLIENT_TAP_TOPIC);
    client.subscribe(MQTT_CLIENT_TYPE_TOPIC);
}

void callback(char* p_topic, uint8_t* p_payload, unsigned int p_length) {
    if (CONFIG_DEBUG) Particle.publish("debug", "INFO: callback", PUBLIC);

    // parse topic
    String topic = ((String)p_topic);
    Log.info(topic);

    // parse payload
    String payload;
    for (uint8_t i = 0; i < p_length; i++) {
        payload.concat((char)p_payload[i]);
    }
    Log.info(payload);

    if (CONFIG_DEBUG) Particle.publish("debug", "INFO: payload=" + payload, PUBLIC);

    // handle request
    if (topic.equals(String(MQTT_CLIENT_LOGIN_TOPIC))) {
        String pin = payload;
        login(pin);
    }
    else if (topic.equals(String(MQTT_CLIENT_TAP_TOPIC))) {
        String key = payload;
        tap(key);
    }
    else if (topic.equals(String(MQTT_CLIENT_TYPE_TOPIC))) {
        String string = payload;
        type(string);
    }
}

#pragma endregion

STARTUP(Keyboard.begin());

void setup()
{
    // wait 4 seconds so serial monitor can reconnect before log messages start coming in
    //delay(4000);

    Log.trace("setup()");
    
    connectWiFi();
    connectMqtt();
    connectParticleCloud();
}

void loop()
{
    Log.trace("loop()");

    if(!WiFi.ready()){
        connectWiFi();
    }

    if(client.isConnected()){
        client.loop();
    }else{
        connectMqtt();
    }
}

void connectWiFi(){
    Log.info("connectWiFi()");
  
    WiFi.on();

    WiFi.disconnect();

    /*
    Photon will remember the 5 most recently set credentials, so you can flash your devices with new credentials
    before changing your network password, and they will connect right up with the new credentials so you don't
    have to manually re-add them all to the network.

    Particle Documentation:
    https://docs.particle.io/reference/device-os/firmware/photon/#setcredentials-
    */
    WiFi.clearCredentials();
    WiFi.setCredentials(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD, WPA2, WLAN_CIPHER_AES);
    WiFi.setCredentials(CONFIG_WIFI_ALTERNATE_SSID, CONFIG_WIFI_ALTERNATE_PASSWORD, WPA2, WLAN_CIPHER_AES);
    
    /*
    This block is for getting stored credentials for debugging purposes. It should normally be left commented out.

    Particle Documentation:
    https://docs.particle.io/reference/device-os/firmware/photon/#getcredentials-
    */
    /*
    WiFiAccessPoint ap[5];
    int found = WiFi.getCredentials(ap, 5);
    for (int i = 0; i < found; i++) {
        Log.info("ssid: %s", ap[i].ssid);
        // security is one of WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2, WLAN_SEC_WPA_ENTERPRISE, WLAN_SEC_WPA2_ENTERPRISE
        Log.info("security: %d", (int) ap[i].security);
        // cipher is one of WLAN_CIPHER_AES, WLAN_CIPHER_TKIP or WLAN_CIPHER_AES_TKIP
        Log.info("cipher: %d", (int) ap[i].cipher);
    }
    */

    WiFi.connect();

    Log.info("connectWiFi(), waitUntil(WiFi.ready)...");

    waitUntil(WiFi.ready);

    Log.info("connectWiFi(), WiFi.ready");
}

void connectMqtt(){
    Log.info("connectMqtt()");

    // the id here includes the time so as to be unique, because many MQTT clients will disconnect 
    // the first connection when you use the same user_id for a new connection
    client.connect("sparkclient_" + String(Time.now()), CONFIG_MQTT_USERNAME, CONFIG_MQTT_PASSWORD);

    if (client.isConnected()) 
    {
        publishAvailability();
        subscribe();
    }
}

void connectParticleCloud(){
    Log.info("connectParticleCloud()");

    /*
    Particle.function(), Particle.variable() and Particle.subscribe() will function as intended whether the cloud is connected or not. 

    Particle Documentation:
    https://docs.particle.io/reference/device-os/firmware/photon/#system-threading-behavior
    */
    Particle.function("login", login);
    Particle.function("tap", tap);
    Particle.function("type", type);

    // this will not block the application thread because we used "SYSTEM_THREAD(ENABLED)"
    // it will attempt to connect to Particle Cloud, and if it cannot, it will keep trying indefinitely
    Particle.connect();
}