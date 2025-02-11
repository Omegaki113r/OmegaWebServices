#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <variant>
#include <vector>

#include "esp_vfs_fat.h"
#include <esp_err.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include <esp_timer.h>
#include <esp_tls.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

#include "OmegaESP32xxNVSController/ESP32xxNVSController.hpp"
#include "OmegaFileSystemController/FileSystemController.hpp"
#include "OmegaWebServices/WebSocket.hpp"
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

const int DATA_SIZE = 64 * 1024;
DMA_ATTR u8 buffer[DATA_SIZE]{0};

extern "C" void app_main(void)
{
    if (const auto err = ::Omega::NVS::init(); eSUCCESS != err)
    {
        OMEGA_LOGE("NVS Init failed");
        return;
    }
    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = "/storage",
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffs_config));

    ::Omega::WiFiController::initialize(::Omega::WiFiController::Mode::eSTATION_MODE);
    ::Omega::WiFiController::connect("Xtronic", "Om3gaki113r");
    ::Omega::WiFiController::wait_for_ip();

    auto [ws, wshandle] = ::Omega::WebServices::WebSocket::Client::Init("ws://192.168.1.4:8080");
start:
    const auto start_time = esp_timer_get_time();
    const auto handle = ::Omega::FileSystemController::open("/storage/log.txt", ::Omega::FileSystemController::OpenMode::eREADING_MODE);
    size_t sent_size = 0;
    bool is_initial = true;
    for (;;)
    {
        if (::Omega::WebServices::WebSocket::Client::is_connected())
        {
            const auto read_size = ::Omega::FileSystemController::read(
                handle,
                ::Omega::FileSystemController::ReadMode::eREAD_CHUNK_MODE,
                buffer, DATA_SIZE);
            if (read_size > 0)
            {
                const char *topic = "/Elma/EABC12";
                if (is_initial)
                {
                    esp_websocket_client_send_bin_partial(wshandle, (const char *)buffer, read_size, portMAX_DELAY);
                    is_initial = false;
                }
                else
                {
                    esp_websocket_client_send_cont_msg(wshandle, (const char *)buffer, read_size, 0);
                }
                sent_size += read_size;
            }
            else
            {
                esp_websocket_client_send_fin(wshandle, portMAX_DELAY);
                OMEGA_LOGI("%.1f MB | %lld ms", ((float)sent_size) / (1024.0f * 1024.0f), (esp_timer_get_time() - start_time) / 1000);
                sent_size = 0;
                ::Omega::FileSystemController::close(handle);
                goto start;
            }
            // delay({0, 0, 5});
        }
        else
        {
            // delay({0, 0, 0, 500});
        }
    }
}
