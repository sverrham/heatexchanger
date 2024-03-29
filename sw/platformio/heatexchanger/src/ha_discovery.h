#ifndef HA_DISCOVERY_H
#define HA_DISCOVERY_H
#include <Arduino.h>
#include <ArduinoJson.h>


class HaDiscovery {
public:
	HaDiscovery(String stateTopic);
    JsonDocument getMQTTPm1p0DiscoveryMsg();
    JsonDocument getMQTTPm2p5DiscoveryMsg();
    JsonDocument getMQTTPm4p0DiscoveryMsg();
    JsonDocument getMQTTPm10p0DiscoveryMsg();
    JsonDocument getMQTTTemperatureDiscoveryMsg();
    JsonDocument getMQTTHumidityDiscoveryMsg();
    JsonDocument getMQTTVocIndexDiscoveryMsg();
    JsonDocument getMQTTNoxIndexDiscoveryMsg();

    String getDiscoveryTopicPm1p0();
    String getDiscoveryTopicPm2p5();
    String getDiscoveryTopicPm4p0();
    String getDiscoveryTopicPm10p0();
    String getDiscoveryTopicTemperature();
    String getDiscoveryTopicHumidity();
    String getDiscoveryTopicVocindex();
    String getDiscoveryTopicNoxindex();	

    void setDeviceInfo(String serialNumber, String hw_version, String sw_version);
private:

	String  _stateTopic;
    JsonDocument _devDoc;

};
#endif




