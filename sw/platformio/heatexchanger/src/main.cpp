

#include <Arduino.h>


#include <PubSubClient.h>
#include <ESPAsyncWiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

#include "ha_discovery.h"

Preferences prefs;

AsyncWebServer server(80);
DNSServer dns;


Adafruit_ADS1115 ads;

JsonDocument sensordata;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// This is the topic this program will send the state of this device to.
String stateTopic;
String mqttServerIp;
int mqttServerPort;
boolean mqttEnabled;

int fanSpeed;
unsigned long fanCycleTime;   


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

const char* TOPIC_MESSAGE = "Mqtt topic";
const char* SERVER_IP_MESSAGE = "Mqtt Server";
const char* SERVER_PORT_MESSAGE = "Mqtt port";
const char* MQTT_ENABLED_MESSAGE = "mqtt_enabled";
const char* FAN_SPEED_MESSAGE = "fan_speed";
const char* FAN_CYCLE_TIME_MESSAGE = "fan_cycle_time";

String genHtml(String stateTopic, String mqttServerIp, String mqttServerPort, bool mqttEnabled, String fanSpeed, String fanCycleTime) {
    String html;
    html += R"(<!DOCTYPE HTML><html><head>)";
    html += R"(<title>Heat exchanger</title>)";
    html += R"(<meta name="viewport" content="width=device-width, initial-scale=1">)";
    html += R"(</head><body>)";
    html += R"(<form action="/get">)";
    html += R"(<input type="text" name="Mqtt topic" value=")" + stateTopic + R"(">)";
    html += R"(<label for="Mqtt topic"> MQTT Topic</label><br>)";
    html += R"(<input type="text" name="Mqtt Server" value=")" + mqttServerIp + R"(">)";
    html += R"(<label for="Mqtt Server"> MQTT Server IP</label><br>)";
    html += R"(<input type="text" name="Mqtt port" value=")" + mqttServerPort + R"(">)";
    html += R"(<label for="Mqtt port"> MQTT Server Port</label><br>)";
    if (mqttEnabled) {
    html += R"(<input type="checkbox" name="mqtt_enabled" value="Yes" checked>)";
    } else {
        html += R"(<input type="checkbox" name="mqtt_enabled" value="Yes">)";
    }
    html += R"(<label for="mqtt_enabled"> MQTT Enabled</label><br><br>)";
    html += R"(<input type="submit" value="Submit">)";
    html += R"(</form><br>)";
    html += "<h1 class=\"label\">MQTT Config</h1>";
    html += "MQTT Topic: " + stateTopic + "<br>";
    html += "MQTT Server: " + mqttServerIp + "<br>";
    html += "MQTT Port: " + mqttServerPort + "<br>";
    String enabled = mqttEnabled ? "Yes" : "No";
    html += "MQTT Enabled: " + enabled + "<br>";
    html += R"(<p><a href="/data">Json sensor data</a></p><br>)";
    //
    html += "<h1 class=\"label\">FAN Control</h1>";
    html += R"(<form action="/get">)";
    html += R"(<input type="text" name="fan_speed" value=")" + fanSpeed + R"(">)";
    html += R"(<label for="Mqtt topic"> Fan speed</label><br>)";
    html += R"(<input type="text" name="fan_cycle_time" value=")" + fanCycleTime + R"(">)";
    html += R"(<label for="Mqtt topic"> Fan cycle time Seconds</label><br>)";
    html += R"(<input type="submit" value="Submit">)";
    html += R"(</form><br>)";
 
    html += R"()";


    html += "</body></html>";

    return html;
}


void publishMQTT(JsonDocument doc, String topic_dev) {
    // Serialize the JSON document to a char buffer
    char jsonBuffer[512];
    size_t n = serializeJson(doc, jsonBuffer);
    const char* payload = jsonBuffer;
    mqttClient.publish(topic_dev.c_str(), jsonBuffer, n);
    Serial.println("Published message: "+ topic_dev + String(payload));
}

// void sendMQTT(SensirionMeasurement data) {
//     JsonDocument doc;
  
//     // Send the sensirion data, to environment/sensirion/technical/
//     // String topic_dev = "environment/sensirion/garage";

//     doc["pm1p0"] = data.massConcentrationPm1p0;
//     doc["pm2p5"] = data.massConcentrationPm2p5;
//     doc["pm4p0"] = data.massConcentrationPm4p0;
//     doc["pm10p0"] = data.massConcentrationPm10p0;
//     doc["humidity"] = data.ambientHumidity;
//     doc["temperature"] = data.ambientTemperature;
//     doc["vocIndex"] = data.vocIndex;
//     doc["noxIndex"] = data.noxIndex;

//     publishMQTT(doc, stateTopic);
    
//     sensordata = doc;
// }

void reconnectMQTT() {
    String mac_addr = WiFi.macAddress();

    if (!mqttClient.connected()) {
        mqttClient.setServer(mqttServerIp.c_str(), mqttServerPort);
        while (!mqttClient.connect(mac_addr.c_str())) {
            Serial.println("MQTT Not Connected");
            delay (1000);
        }
        // Send discovery messages
        // HaDiscovery ha_discovery(stateTopic);
        // ha_discovery.setDeviceInfo(getSen5xSerialNumber(), getSen5xHwVersion(), getSen5xSwVersion());
        // publishMQTT(ha_discovery.getMQTTPm1p0DiscoveryMsg(), ha_discovery.getDiscoveryTopicPm1p0());
        // publishMQTT(ha_discovery.getMQTTPm2p5DiscoveryMsg(), ha_discovery.getDiscoveryTopicPm2p5());
        // publishMQTT(ha_discovery.getMQTTPm4p0DiscoveryMsg(), ha_discovery.getDiscoveryTopicPm4p0());
        // publishMQTT(ha_discovery.getMQTTPm10p0DiscoveryMsg(), ha_discovery.getDiscoveryTopicPm10p0());
        // publishMQTT(ha_discovery.getMQTTTemperatureDiscoveryMsg(), ha_discovery.getDiscoveryTopicTemperature());
        // publishMQTT(ha_discovery.getMQTTHumidityDiscoveryMsg(), ha_discovery.getDiscoveryTopicHumidity());
        // publishMQTT(ha_discovery.getMQTTVocIndexDiscoveryMsg(), ha_discovery.getDiscoveryTopicVocindex());
        // publishMQTT(ha_discovery.getMQTTNoxIndexDiscoveryMsg(), ha_discovery.getDiscoveryTopicNoxindex());

    }
    Serial.println("MQTT Connected");
}

// Function to calculate tempearture from NTC resistor
// In this function the NTC is 10kohm
// The voltage is measured over the NTC resistor, so 10Kohm fixed resistor from VDD
// Connected with NTC resistor to GND.
// Taken from this example: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
float calc_temp(float Vo) {
  float R1 = 10000;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  R2 = R1 / ((3.3 / Vo) - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  
  // Serial.print("temp: ");
  Serial.print(T);
  return T;
}

void setup() {

    Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }

    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wm(&server,&dns);

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    //res = wm.autoConnect("SensirionAP","sens"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

    mqttClient.setBufferSize(512);

    prefs.begin("Heat exchanger");
    stateTopic = prefs.getString("mqttStateTopic", "hvac/heatexchager");
    mqttServerIp = prefs.getString("mqttServerIp", "192.168.1.10");
    mqttServerPort = prefs.getInt("mqttServerPort", 1883);
    mqttEnabled = prefs.getBool("mqttEnabled", true);
    fanSpeed = prefs.getInt("fanSpeed", 255);
    fanCycleTime = prefs.getULong("fanCycleTime", 90);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", genHtml(stateTopic, mqttServerIp.c_str(), String(mqttServerPort), mqttEnabled, String(fanSpeed), String(fanCycleTime)));
    });
    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String topic = stateTopic;
        mqttEnabled = false;
        if (request->hasParam(TOPIC_MESSAGE)) {
            topic = request->getParam(TOPIC_MESSAGE)->value();
            stateTopic = topic;
            prefs.putString("mqttStateTopic", topic);
        } 
        if (request->hasParam(SERVER_IP_MESSAGE)) {
            mqttServerIp = request->getParam(SERVER_IP_MESSAGE)->value();
            prefs.putString("mqttServerIp", mqttServerIp);
        }
        if (request->hasParam(SERVER_PORT_MESSAGE)) {
            mqttServerPort = atoi(request->getParam(SERVER_PORT_MESSAGE)->value().c_str());
            prefs.putInt("mqttServerPort", mqttServerPort);
        }
        if (request->hasParam(MQTT_ENABLED_MESSAGE)) {
            mqttEnabled = request->getParam(MQTT_ENABLED_MESSAGE)->value() == "Yes";
        }
        if (request->hasParam(FAN_SPEED_MESSAGE)) {
            fanSpeed = atoi(request->getParam(FAN_SPEED_MESSAGE)->value().c_str()); 
            prefs.putInt("fanSpeed", fanSpeed);
        }
        if (request->hasParam(FAN_CYCLE_TIME_MESSAGE)) {
            fanCycleTime = atoi(request->getParam(FAN_CYCLE_TIME_MESSAGE)->value().c_str()); 
            prefs.putULong("fanCycleTime", fanCycleTime);
        }


        prefs.putBool("mqttEnabled", mqttEnabled); // Always write this to catch the Enable checkbox not checked also.
        
        request->send(200, "text/html", genHtml(topic, mqttServerIp, String(mqttServerPort), mqttEnabled, String(fanSpeed), String(fanCycleTime)));
    });
    
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        String response;
        serializeJson(sensordata, response);
        request->send(200, "application/json", response);
    });

    server.onNotFound(notFound);
    server.begin();


    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS.");
        while (1);
    }

    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    
    analogWrite(D3, fanSpeed);
    analogWrite(D4, 0);
}

enum FanDirection {
    INN, OUT
};

void loop() {
    // These will be configurable
    
    static FanDirection direction = INN;
    static unsigned long previousMillis = 0;
    static unsigned long previousMillisFan = 0;
    unsigned long currentMillis = millis();
    unsigned long currentMillisFan = millis();
    // SensirionMeasurement data;
    int16_t results;
    float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */


    if (mqttEnabled) {
        mqttClient.loop();
    }

    if (currentMillis - previousMillis >= 1000) {
        //restart this TIMER
        previousMillis = currentMillis;
       
        // data = readSen5xData();
        

        results = ads.readADC_Differential_0_1();
        Serial.print("Differential: "); Serial.print(results); Serial.print("("); Serial.print(results * multiplier); Serial.println("mV)");

        float temp = calc_temp(results * multiplier);
        Serial.println("Temp: " + String(temp));

        if (mqttEnabled) {
            reconnectMQTT();
            // sendMQTT(data);
        }
    }

    if (currentMillisFan - previousMillisFan >= fanCycleTime*1000) {
        //restart this TIMER
        previousMillisFan = currentMillisFan;
       
        // Setup one timer for the fans to change directions and set speed
        // Each time this trips we should change direction of the fans.
        if (direction == INN) {
            Serial.println("INN");
            analogWrite(D4, 0);
            analogWrite(D3, fanSpeed);
            direction = OUT;
        } else if (direction == OUT) {
            Serial.println("OUT");
            analogWrite(D4, fanSpeed);
            analogWrite(D3, 0);
            direction = INN;
        }
    }
}
