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
    File connectors = LittleFS.open(ModelController::BaseConnector::defaultConfigFile);
    Serial.println(connectors.readString().c_str());
    connectors.close();

    ModelController::BaseModule::InitConfig();
}

int i = 0;
void loop()
{
    Logger::debug("start loop");

    ModelController::LoopEvent::Raise();
    WiFiHandler::Check();

    Logger::debug("end loop");
}