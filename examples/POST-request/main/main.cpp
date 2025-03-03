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
#include "OmegaWebServices/Post.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

// #define URL "https://httpbin.org/get"
// #define URL "https://httpbin.org/stream/1"
// #define URL "https://randomuser.me/api/"
#define URL "http://192.168.43.159:3000/uploads/"
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
    ::Omega::WiFiController::wait_for_ip();

    {
        auto [status, data] = ::Omega::WebServices::Request::POST(::Omega::WebServices::ESP32xx())
                                  .url(URL)
                                  .perform();
        for (const auto &[key, value] : data.header)
        {
            OMEGA_LOGI("%s: %s", key, value);
        }
        OMEGA_HEX_LOGI(data.body, data.body_size);
    }
}