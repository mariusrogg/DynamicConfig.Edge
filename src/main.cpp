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

ModelController::Event<std::string> event;
void callback1(std::string val)
{
    Serial.print("1: ");
    Serial.println(val.c_str());
}
void callback2(std::string val)
{
    Serial.print("2: ");
    Serial.println(val.c_str());
}
void callback3(std::string val)
{
    Serial.print("3: ");
    Serial.println(val.c_str());
}
void loopCallback()
{
    Serial.println(millis());
}
ModelController::Event<std::string>::Listener listener1(&event, callback1);
ModelController::Event<std::string>::Listener listener2(&event, callback2);
ModelController::Event<std::string>::Listener* listener3 = nullptr;
ModelController::LoopEvent::LoopListener* loopListener;
void OnModuleOutCreated(std::string modulePath)
{
    Logger::trace("OnModuleOutCreated(" + modulePath + ")");
    ModelController::BaseModuleOut<double>* module = ModelController::BaseModule::GetModule<ModelController::BaseModuleOut<double>>(modulePath);
    if (module != nullptr)
    {
        Logger::trace("ModuleOut created - path:     " + module->GetPath());
        Logger::trace("ModuleOut created - type:     " + ModelController::BaseModule::TypeToString(module->GetType()));
        Logger::trace("ModuleOut created - dataType: " + ModelController::BaseModule::DataTypeToString(module->GetDataType()));
    }
    else
    {
        Logger::trace("ModuleOut created - nullptr");
    }
}

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

    ModelController::BaseModuleOut<double>::ModuleOutCreated.AddCallback(OnModuleOutCreated);

    ModelController::BaseModule::InitConfig();

    event += callback1;
    event += callback2;

    Logger::debug("Double: " + ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(double))));
    Logger::debug("String: " + ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(std::string))));
    Logger::debug("Bool: " + ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(bool))));

    // loopListener = new ModelController::LoopEvent::LoopListener(loopCallback, 2);
    std::string path = "/mqtt/in/double";
    ModelController::MQTTInput<double>* mqttDbl = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>(path, ModelController::BaseModule::ModuleType::eOutput, ModelController::BaseModule::ModuleDataType::eDouble);
    // Serial.println(ModelController::BaseConnector::DataTypeToString(mqttDbl->GetDataType()).c_str());
    ModelController::MQTTInput<double>* mqttDblCpy = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>(path, ModelController::BaseModule::ModuleType::eOutput, ModelController::BaseModule::ModuleDataType::eDouble);
}

int i = 0;
void loop()
{
    Logger::debug("start loop");
    Serial.println("Test");
    ModelController::LoopEvent::Raise();
    WiFiHandler::Check();

    event("Hello from listener");
    // Serial.println(i);
    if (i++ > 10 && listener3 == nullptr)
    {
        listener3 = new ModelController::Event<std::string>::Listener(&event, callback3);
    }
    else if (i > 20)
    {
        delete listener3;
        listener3 = nullptr;
        i = 0;
    }

    std::string path = "/mqtt/in/double";
    ModelController::MQTTInput<double>* mqttDbl = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>(path, ModelController::BaseModule::ModuleType::eOutput, ModelController::BaseModule::ModuleDataType::eDouble);
    double value = mqttDbl->GetValue();
    Logger::debug("MQTT in: " + std::to_string(value));
    // ModelController::BaseConnector::GetConnector<ModelController::OnboardPWM>("/PWM_0")->SetValue(100.0);
    // ModelController::BaseConnector::GetConnector<ModelController::OnboardPWM>("/PWM_1")->SetValue(value);
    ModelController::MQTTOutput<int32_t>* mqttInt = ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<int32_t>>("/mqtt/test/value", ModelController::BaseModule::ModuleType::eInput, ModelController::BaseModule::ModuleDataType::eInt32);
    mqttInt->SetValue(i);
    ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<double>>("/mqtt/const/twenty", ModelController::BaseModule::ModuleType::eInput, ModelController::BaseModule::ModuleDataType::eDouble)->SetValue(20);
    // ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<std::string>>("/mqtt/const/string", ModelController::BaseModule::ModuleType::eInput, ModelController::BaseModule::ModuleDataType::eString)->SetValue("string");

    Logger::debug("end loop");
}