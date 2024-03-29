

#include <Arduino.h>
#include <ArduinoJson.h>
#include "ha_discovery.h"

HaDiscovery::HaDiscovery(String stateTopic) {
	_stateTopic = stateTopic;
    setDeviceInfo("NA", "NA", "NA");
}

void HaDiscovery::setDeviceInfo(String serialNumber, String hw_version, String sw_version) {
    _devDoc["ids"] = serialNumber; //identifiers
    _devDoc["mf"] = "Sham(Sensirion)"; //manufacturer
    _devDoc["mdl"] = "SEN55";  //model
    _devDoc["name"] = "Sensirion SEN55";
    _devDoc["hw"] = hw_version; //hw_version
    _devDoc["sw"] = sw_version; //sw_version
}

String HaDiscovery::getDiscoveryTopicPm1p0() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/pm1p0/config";
}

JsonDocument HaDiscovery::getMQTTPm1p0DiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;
    

    doc["name"] = "Env " + String(sensorNumber) + " Pm1p0";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "μg/mᵌ";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.pm1p0|default(0) }}";
    doc["uniq_id"] = String(ESP.getChipId()) + "_pm1p0";
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicPm2p5() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/pm2p5/config";
}

JsonDocument HaDiscovery::getMQTTPm2p5DiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " Pm2p5";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "μg/mᵌ";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.pm2p5|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_pm2p5";
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicPm4p0() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/pm4p0/config";
}

JsonDocument HaDiscovery::getMQTTPm4p0DiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " Pm4p0";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "μg/mᵌ";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.pm4p0|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_pm4p0";;
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicPm10p0() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/pm10p0/config";
}

JsonDocument HaDiscovery::getMQTTPm10p0DiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " Pm10p0";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "μg/mᵌ";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.pm10p0|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_pm10p0";;
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicTemperature() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/temperature/config";
}

JsonDocument HaDiscovery::getMQTTTemperatureDiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " Temperature";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "⁰C";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.temperature|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_temp";;
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicHumidity() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/humidity/config";
}

JsonDocument HaDiscovery::getMQTTHumidityDiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " Humidity";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "%RH";
    doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.humidity|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_humid";
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicVocindex() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/vocindex/config";
}

JsonDocument HaDiscovery::getMQTTVocIndexDiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " VocIndex";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "";
    // doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.vocIndex|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_voci";
    doc["device"] = _devDoc;

    return doc;
}

String HaDiscovery::getDiscoveryTopicNoxindex() {
    int sensorNumber = ESP.getChipId();
    return "homeassistant/sensor/env_sensor_" + String(sensorNumber) + "/noxindex/config";
}

JsonDocument HaDiscovery::getMQTTNoxIndexDiscoveryMsg() {
    // My numeric sensor ID, you can change this to whatever suits your needs
    int sensorNumber = ESP.getChipId();
    JsonDocument doc;

    doc["name"] = "Env " + String(sensorNumber) + " NoxIndex";
    doc["stat_t"]   = _stateTopic;
    doc["unit_of_meas"]   = "";
    // doc["sug_dsp_prc"] = 2; //         'suggested_display_precision',
    doc["frc_upd"] = true;
    doc["val_tpl"] = "{{ value_json.noxIndex|default(0) }}";

    doc["uniq_id"] = String(ESP.getChipId()) + "_noxi";;
    doc["device"] = _devDoc;

    return doc;
}