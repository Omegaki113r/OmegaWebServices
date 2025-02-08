#include <cstring>
#include <memory>
#include <stdio.h>
#include <variant>
#include <vector>

#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include <cJSON.h>

// #include "OmegaWebServices/WebServices.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWebServices/WebSocket.hpp"
#include "OmegaWiFiController/WiFiController.hpp"

// #define URL "wss://ws.iot.arc.lv?type=DEVICE&deviceId=85395610-99d3-468b-b296-98ef0134fa36&password=mFOHDEM2z73MFLrO"
// #define URL_LEN strlen(URL)

#define URL "ws://127.0.0.1:8765"
#define URL_LEN strlen(URL)

char app_token[255] = {0};

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

    if (eSUCCESS != ::Omega::WebServices::WebSocket::Client::Init(URL))
    {
        OMEGA_LOGE("::Omega::WebServices::WebSocket(%s) failed", URL);
        return;
    }
}

constexpr const char *SERVER_INFO_STR = "SERVER_INFO";
constexpr size_t SERVER_INFO_LEN = std::strlen(SERVER_INFO_STR);

internal inline void handle_websocket_data(const u8 *data, const size_t data_length)
{
    const std::unique_ptr<cJSON, cJSONDeleter> parent_obj{cJSON_ParseWithLength(reinterpret_cast<const char *>(data), data_length)};
    if (nullptr == parent_obj)
    {
        return;
    }
    const auto topic_obj = cJSON_GetObjectItem(parent_obj.get(), "topic");
    const auto topic = cJSON_GetStringValue(topic_obj);
    if (!std::memcmp(topic, SERVER_INFO_STR, SERVER_INFO_LEN))
    {
        const auto payload_obj = cJSON_GetObjectItem(parent_obj.get(), "payload");
        const auto token_obj = cJSON_GetObjectItem(payload_obj, "token");
        const auto token = cJSON_GetStringValue(token_obj);
        std::memcpy(app_token, token, std::strlen(token));

        OMEGA_LOGI("App Token: %s", app_token);
    }
}

void ::Omega::WebServices::WebSocket::Client::on_data(const u8 *data, const size_t data_len)
{
    OMEGA_HEX_LOGD((u8 *)data, data_len);
    handle_websocket_data(data, data_len);
}