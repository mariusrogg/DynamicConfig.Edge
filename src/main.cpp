#include "LittleFS.h"
#include "OnboardPWM.hpp"
#include "BaseConnector.hpp"
#include "WiFiHandler.hpp"
#include "EventHandling.hpp"
#include "MQTTOutput.hpp"
#include "MQTTInput.hpp"
#include "MQTTClient.hpp"
#include "LoopEvent.hpp"

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

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\n");
    Serial.println("started");
    Serial.println("\n\n");

    // Check if Filesystem was initialized
    if (!LittleFS.begin(false, ""))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }
    File connectors = LittleFS.open(ModelController::BaseConnector::defaultConfigFile);
    Serial.println(connectors.readString().c_str());
    connectors.close();

    ModelController::BaseConnector::InitConfig();

    event += callback1;
    event += callback2;

    Serial.print("Double: ");
    Serial.println(ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(double))).c_str());
    Serial.print("String: ");
    Serial.println(ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(std::string))).c_str());
    Serial.print("Bool: ");
    Serial.println(ModelController::BaseConnector::DataTypeToString(ModelController::BaseConnector::GetDataTypeById(typeid(bool))).c_str());

    // loopListener = new ModelController::LoopEvent::LoopListener(loopCallback, 2);
    ModelController::MQTTInput<double>* mqttDbl = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>("/mqtt/in/double", ModelController::BaseConnector::ConnectorType::eInput, ModelController::BaseConnector::ConnectorDataType::eDouble);
    Serial.println(ModelController::BaseConnector::DataTypeToString(mqttDbl->GetDataType()).c_str());
    ModelController::MQTTInput<double>* mqttDblCpy = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>("/mqtt/in/double", ModelController::BaseConnector::ConnectorType::eInput, ModelController::BaseConnector::ConnectorDataType::eDouble);
}

int i = 0;
void loop()
{
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
    ModelController::MQTTInput<double>* mqttDbl = ModelController::BaseConnector::GetConnector<ModelController::MQTTInput<double>>("/mqtt/in/double", ModelController::BaseConnector::ConnectorType::eInput, ModelController::BaseConnector::ConnectorDataType::eDouble);
    double value = mqttDbl->GetValue();
    Serial.print("MQTT in: ");
    Serial.println(value);
    ModelController::BaseConnector::GetConnector<ModelController::OnboardPWM>("/PWM_0")->SetValue(100.0);
    ModelController::BaseConnector::GetConnector<ModelController::OnboardPWM>("/PWM_1")->SetValue(100.0);
    ModelController::MQTTOutput<int32_t>* mqttInt = ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<int32_t>>("/mqtt/test/value", ModelController::BaseConnector::ConnectorType::eOutput, ModelController::BaseConnector::ConnectorDataType::eInt32);
    mqttInt->SetValue(i);
    ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<double>>("/mqtt/const/twenty", ModelController::BaseConnector::ConnectorType::eOutput, ModelController::BaseConnector::ConnectorDataType::eDouble)->SetValue(20);
    // ModelController::BaseConnector::GetConnector<ModelController::MQTTOutput<std::string>>("/mqtt/const/string", ModelController::BaseConnector::ConnectorType::eOutput, ModelController::BaseConnector::ConnectorDataType::eString)->SetValue("string");

    sleep(1);
}