#include "LittleFS.h"
#include "WiFiHandler.hpp"
#include "BaseModule.hpp"
#include "LoopEvent.hpp"
#include "Logger.hpp"
#include "ConfigFile.hpp"
#include "ConfigAPI.hpp"

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
    File connectors = LittleFS.open(ModelController::BaseModule::defaultConfigFile);
    Serial.println(connectors.readString().c_str());
    connectors.close();

    ModelController::ConfigFile::Load();

    ModelController::BaseModule::UpdateConfig(ModelController::ConfigFile::GetConfig("").as<JsonObject>());
}

int i = 0;
void loop()
{
    Logger::debug("start loop");

    ModelController::LoopEvent::Raise();
    ModelController::ConfigAPI::Handle(WiFiHandler::Check());

    Logger::debug("end loop");
}