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

// #include "OmegaWebServices/WebServices.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

// #define URL "https://httpbin.org/get"
// #define URL "https://httpbin.org/stream/1"
#define URL "https://randomuser.me/api/"
// #define URL "http://192.168.1.4:3000/uploads/"
// #define URL "http://127.0.0.1:3000/uploads/"
#define URL_LEN strlen(URL)

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

    // const auto callback = [](const uint8_t *data, const size_t data_len)
    // {
    //     OMEGA_LOGD("Length: %d", data_len);
    //     OMEGA_LOGD("Data: %s", (const char *)data);
    // };
    const auto [status, data] = ::Omega::WebServices::Requests::GET(URL, callback);
    if (eSUCCESS != status)
    {
        OMEGA_LOGE("Request failed");
        return;
    }
    // for (const auto &[key, value] : data.header)
    // {
    //     OMEGA_LOGD("%s : %s", key.c_str(), value.c_str());
    // }
    // OMEGA_HEX_LOGI(data.body, data.body_size);
}
