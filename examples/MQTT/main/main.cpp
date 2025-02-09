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
#define BROKER_USERNAME "Ytronic"
#define BROKER_PASSWORD "@M0irana"

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

    // const auto state = ::Omega::WebServices::MQTT::CLIENT::connect(BROKER_URL, BROKER_PORT,
    //                                                                BROKER_USERNAME, BROKER_PASSWORD);
    const auto state = ::Omega::WebServices::MQTT::CLIENT::connect(BROKER_URL, BROKER_PORT);
    for (;;)
    {
        delay({0, 0, 5});
        const char *topic = "/Elma/EABC12";
        const char *data = "world";
        ::Omega::WebServices::MQTT::CLIENT::publish(topic, data, std::strlen(data));
        OMEGA_LOGI("[%s] => %s", topic, data);
    }
}
