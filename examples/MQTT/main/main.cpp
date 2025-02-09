#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

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
{ OMEGA_LOGI("On Connected"); };
const auto on_data = [](const u8 *data, size_t data_length)
{ OMEGA_LOGI("On Data"); };
const auto on_disconnected = []()
{ OMEGA_LOGI("On Disconnected"); };

auto client = ::Omega::WebServices::MQTT::Client(BROKER_URL, BROKER_PORT)
                  .on_connected(on_connected)
                  .on_data(on_data)
                  .on_disconnected(on_disconnected);

extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ::Omega::WiFiController::initialize(::Omega::WiFiController::Mode::eSTATION_MODE);
    ::Omega::WiFiController::connect("Xtronic", "Om3gaki113r");
    ::Omega::WiFiController::wait_for_ip();

    client.connect();
    for (;;)
    {
        if (client.is_connected() == ::Omega::WebServices::MQTT::State::eCONNECTED)
        {
            const char *topic = "/Elma/EABC12";
            const char *data = "world";
            client.publish(topic, data, std::strlen(data), 1);
            OMEGA_LOGI("[%s] => %s", topic, data);
        }
        delay({0, 0, 5});
    }
}
