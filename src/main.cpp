#include "LittleFS.h"
#include "OnboardPWM.hpp"
#include "BaseConnector.hpp"
#include "WiFiHandler.hpp"
#include "EventHandling.hpp"
#include "MQTTOutput.hpp"
#include "MQTTInput.hpp"
#include "MQTTClient.hpp"
#include "LoopEvent.hpp"
#include "Logger.hpp"
#include "API.hpp"
#include "ConfigFile.hpp"

void setup()
{
    Serial.begin(115200);
    Logger::info("started");

    // Check if Filesystem was initialized
    if (!LittleFS.begin(false, ""))
    {
        Logger::fatal("LittleFS failed");
        return;
    }
    // API::Initialize();

    ModelController::ConfigFile::Load();

    // ModelController::ConfigFile::Remove("/", false);
    // ModelController::ConfigFile::Remove("/root", false);
    // ModelController::ConfigFile::Remove("//root//", false);
    // ModelController::ConfigFile::Remove("test", false);
    // ModelController::ConfigFile::Remove("/root/test", false);
    // ModelController::ConfigFile::Remove("parent/child///", false);
    // ModelController::ConfigFile::SetConfig("parent/child/child1/child2/child3", "test");
    // ModelController::ConfigFile::SetConfig("parent/child/child1/child2/child4", "tes1");
    // ModelController::ConfigFile::SetConfig("parent2/child/child1/child2/child4", "asdf");
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    // ModelController::ConfigFile::Remove("parent/child/child1/child2/child3", true);
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    // ModelController::ConfigFile::Remove("parent/child/child1/child2/child4", true);
    // ModelController::ConfigFile::Remove("parent/child/child1/child2/child5", true);
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
//
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    // ModelController::ConfigFile::SetConfig("/test/wifi/ssid", "ssid");
    // ModelController::ConfigFile::SetConfig("/test/wifi/password", "passwd");
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    // ModelController::ConfigFile::Remove("/test/wifi");
    // ModelController::ConfigFile::Remove("/test/wifi/");
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
//
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfig("/root/wifi/ssid").as<std::string>().c_str());
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfig("/root/wifi").as<std::string>().c_str());
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfig("/root/Connectors").as<std::string>().c_str());
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfig("/root/Connector").as<std::string>().c_str());
    // Serial.println("_______________________________________________________________________________________");
    // Serial.println(ModelController::ConfigFile::GetConfig("/root/").as<std::string>().c_str());
    // Serial.println("_______________________________________________________________________________________");

    File connectors = LittleFS.open(ModelController::ConfigFile::defaultConfigFile);
    Serial.println(connectors.readString().c_str());
    connectors.close();

    ModelController::BaseModule::UpdateConfig(ModelController::ConfigFile::GetConfig("").as<JsonObject>());

    Serial.println("_________________________________________________________________________");
    Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    Serial.println("_________________________________________________________________________");


    DynamicJsonDocument doc(1000);
    doc["type"] = "gain";
    doc["in"] = "/root/Connectors/mqtt/in/double4";
    doc["out"] = "/root/Connectors/mqtt/out/double4";
    doc["gain"] = 1;
    JsonObject Gain2 = doc.createNestedObject("Gain2");
    Gain2["type"] = "gain";
    Gain2["in"] = "/root/Connectors/mqtt/in/double20";
    Gain2["out"] = "/root/Connectors/mqtt/out/double20";
    Gain2["gain"] = 2;
    ModelController::BaseModule::Update("/root/Processors/Gain1", doc);
    Serial.println("_________________________________________________________________________");
    Serial.println(ModelController::ConfigFile::GetConfigFile().c_str());
    Serial.println("_________________________________________________________________________");

    Serial.read();
}

int i = 0;

void loop()
{
    Logger::debug("start loop");

    ModelController::LoopEvent::Raise();
    API::Handle(WiFiHandler::Check());

    Logger::debug("end loop");
}