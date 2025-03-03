#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <nvs_flash.h>

#include <esp_http_client.h>

// #include "OmegaWebServices/WebServices.hpp"
#include "OmegaWebServices/ESP32xx.hpp"
#include "OmegaWebServices/Get.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

// #define URL "https://httpbin.org/get"
// #define URL "https://httpbin.org/stream/1"
#define URL "https://randomuser.me/api/"
// #define URL "http://192.168.1.4:3000/uploads/"
// #define URL "http://127.0.0.1:3000/uploads/"
#define URL_LEN std::strlen(URL)

#define URL_IMG "https://randomuser.me/api/portraits/thumb/women/27.jpg"
#define URL_IMG_LEN std::strlen(URL_IMG)

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
    ::Omega::WiFiController::connect("GalaxyS9+71b0", "bqwk9667");
    if (const auto [state, node] = ::Omega::WiFiController::wait_for_ip(); eSUCCESS != state)
    {
        OMEGA_LOGE("Waiting for IP failed");
        return;
    }

    OMEGA_LOGI("Starting get request");
    {
        const auto start_time = esp_timer_get_time();
        const auto chunked_callback = [&](const u8 *data, size_t data_length)
        {
            // OMEGA_LOGW("Chunk received with length: %d", data_length);
            // OMEGA_HEX_LOGI((void *)data, data_length);
        };
        auto [status, data] = ::Omega::WebServices::Request::GET(::Omega::WebServices::ESP32xx())
                                  .url("http://192.168.43.159:3000")
                                  .perform_chunked(chunked_callback);
        OMEGA_LOGI("execution time: %.1fs", ((float)esp_timer_get_time() - (float)start_time) / (1000.0f * 1000.0f));
        for (const auto &[key, value] : data.header)
        {
            OMEGA_LOGI("%s: %s", key, value);
        }
    }

    // {
    //     const auto start_time = esp_timer_get_time();
    //     auto [status, data] = ::Omega::WebServices::Request::GET(::Omega::WebServices::ESP32xx())
    //                               .url(URL)
    //                               .perform();
    //     for (const auto &[key, value] : data.header)
    //     {
    //         OMEGA_LOGI("%s: %s", key, value);
    //     }
    //     OMEGA_HEX_LOGI(data.body, data.body_size);
    // }
}