#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>
#include <chrono>
#include <thread>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/x86_64.hpp"
#include "OmegaWebServices/MQTT.hpp"

#define URL "192.168.43.159:1883"
#define URL_LEN std::strlen(URL)

int main(void)
{
    const auto on_connected = []() { OMEGA_LOGI("Connected"); };
    const auto on_data = [](const u8* data, size_t data_length) {};
    const auto on_disconnected = []() { OMEGA_LOGI("Disconnected"); };

    auto  mqtt_client = ::Omega::WebServices::MQTT::Client(::Omega::WebServices::x86_64())
                                .url("192.168.43.159:1883")
                                .on_connected(on_connected)
                                .on_data(on_data)
                                .on_disconnected(on_disconnected);
    if (eFAILED == mqtt_client.connect()) 
    {
        OMEGA_LOGE("Connection failed");
        return -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    mqtt_client.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}