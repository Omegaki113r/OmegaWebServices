#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include "OmegaESP32xxNVSController/ESP32xxNVSController.hpp"
#include "OmegaWebServices/MQTT.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

// #define BROKER_URL "mail.aisens.se"
// #define BROKER_URL_LEN strlen(BROKER_URL)

// #define BROKER_PORT 54100
// #define BROKER_USERNAME "Ytronic"
// #define BROKER_PASSWORD "@M0irana"

#define BROKER_URL "broker.hivemq.com"
#define BROKER_URL_LEN strlen(BROKER_URL)
#define BROKER_PORT 1883

const auto on_connected = []()
{
    OMEGA_LOGI("On Connected");
};
const auto on_data = [](const u8 *data, size_t data_length)
{
    OMEGA_LOGI("On Data");
};
const auto on_disconnected = []()
{
    OMEGA_LOGI("On Disconnected");
};

auto client = ::Omega::WebServices::MQTT::Client("192.168.1.2", 1883)
                  .on_connected(on_connected)
                  .on_data(on_data)
                  .on_disconnected(on_disconnected);

const int DATA_SIZE = 30 * 1024;
const char *generateRandomData()
{
    static char randomData[DATA_SIZE];                                                                        // Make sure the data stays in scope
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?"; // Characters to pick from
    const int charsetSize = sizeof(charset) - 1;                                                              // Exclude the null-terminator

    for (int i = 0; i < DATA_SIZE - 1; ++i)
    { // Fill the array with random data
        randomData[i] = charset[rand() % charsetSize];
    }

    randomData[DATA_SIZE - 1] = '\0'; // Null-terminate the string
    return randomData;                // Return the pointer to the generated data
}
const char *randomString = generateRandomData();

extern "C" void app_main(void)
{
    if (const auto err = ::Omega::NVS::init(); eSUCCESS != err)
    {
        OMEGA_LOGE("NVS Init failed");
        return;
    }

    std::srand(std::time(0));

    ::Omega::WiFiController::initialize(::Omega::WiFiController::Mode::eSTATION_MODE);
    ::Omega::WiFiController::connect("Xtronic", "Om3gaki113r");
    ::Omega::WiFiController::wait_for_ip();

    client.connect();
    for (;;)
    {
        if (client.is_connected() == ::Omega::WebServices::MQTT::State::eCONNECTED)
        {
            const char *topic = "/Elma/EABC12";
            const char *data = "";
            client.publish(topic, randomString, std::strlen(randomString), 1);
            // OMEGA_LOGI("[%s] => %s", topic, data);
        }
        delay({0, 0, 0, 100});
    }
}
