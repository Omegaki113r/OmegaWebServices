#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_log.h>
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
    ::Omega::WiFiController::wait_for_ip();

    // const auto callback = [](const uint8_t *data, const size_t data_len)
    // {
    //     OMEGA_LOGD("Length: %d", data_len);
    //     OMEGA_LOGD("Data: %s", (const char *)data);
    // };
    // const auto [status, data] = ::Omega::WebServices::Requests::GET(URL, callback);
    // if (eSUCCESS != status)
    // {
    //     OMEGA_LOGE("Request failed");
    //     return;
    // }

    {
        for (size_t i = 0; i < 50; ++i)
        {
            const auto start = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
            {
                auto get_request = ::Omega::WebServices::Request::GET(::Omega::WebServices::ESP32xx())
                                       .url(URL);
                auto [status, data] = get_request
                                          .perform();
                for (const auto &[key, value] : data.header)
                {
                    OMEGA_LOGI("%s: %s", key, value);
                }
                OMEGA_HEX_LOGI(data.body, data.body_size);
            }
            delay({0, 0, 1});
            OMEGA_LOGI("%d", start - heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        }
    }
    // OMEGA_LOGI("%d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    // auto another_request = ::Omega::WebServices::Request::GET(::Omega::WebServices::ESP32xx())
    //                            .url("https://randomuser.me/api/portraits/thumb/women/27.jpg");
    // another_request.perform();

    // for (const auto &[key, value] : data.header)
    // {
    //     OMEGA_LOGD("%s : %s", key.c_str(), value.c_str());
    // }
    // OMEGA_HEX_LOGI(data.body, data.body_size);

    // for (size_t i = 0; i < 50; ++i)
    // {
    //     const auto start = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    //     {
    //         const esp_http_client_config_t config{URL};
    //         const esp_http_client_handle_t http_handle = esp_http_client_init(&config);
    //         esp_http_client_perform(http_handle);
    //         esp_http_client_cleanup(http_handle);
    //     }
    //     delay({0, 0, 2});
    //     OMEGA_LOGI("%d", start - heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    // }
    for (;;)
    {
        OMEGA_LOGE("%d", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        delay({0, 0, 30});
    }
}