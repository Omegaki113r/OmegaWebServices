#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <nvs_flash.h>

#include <esp_http_client.h>

#include "OmegaWebServices/ESP32xx.hpp"
#include "OmegaWebServices/Get.hpp"
#include "OmegaWebServices/Post.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

#define URL "http://192.168.43.159:3000/log"
#define URL_LEN std::strlen(URL)

extern "C" void app_main(void)
{
    {
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
        {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);
    }

    ::Omega::WiFiController::initialize(::Omega::WiFiController::Mode::eSTATION_MODE);
    // ::Omega::WiFiController::connect("GalaxyS9+71b0", "bqwk9667");
    ::Omega::WiFiController::connect("Xtronic", "Omegaki113r");
    if (const auto [state, node] = ::Omega::WiFiController::wait_for_ip(); eSUCCESS != state)
    {
        OMEGA_LOGE("Waiting for IP failed");
        return;
    }

    const auto start_time = esp_timer_get_time();
    const auto chunked_callback = [&](const u8 *data, size_t data_length)
    {
        // OMEGA_LOGW("Chunk received with length: %d", data_length);
        // OMEGA_HEX_LOGI((void *)data, data_length);
    };
    auto [status, data] = ::Omega::WebServices::Request::POST(::Omega::WebServices::ESP32xx())
                              .url(URL)
                              .perform();
    if (eSUCCESS != status)
    {
        OMEGA_LOGE("Post request has failed");
        return;
    }

    OMEGA_LOGI("[%.1f MB] execution time: %.1fs",
               static_cast<float>(data.body_size) / (1000.0f * 1000.0f),
               (static_cast<float>(esp_timer_get_time()) - static_cast<float>(start_time)) / (1000.0f * 1000.0f));
    for (const auto &[key, value] : data.header)
    {
        OMEGA_LOGI("%s: %s", key, value);
    }
    OMEGA_HEX_LOGI(data.body, data.body_size);
    OMEGA_LOGI("Request Status: %d", data.status_code);
}