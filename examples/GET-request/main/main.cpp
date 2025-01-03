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

#include "OmegaWebServices/WebServices.hpp"

#define URL "https://httpbin.org/get"
#define URL_LEN strlen(URL)

esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
    {
        char *data = (char *)malloc(sizeof(char) * (evt->data_len + 1));
        std::strncpy(data, (const char *)evt->data, evt->data_len);
        data[evt->data_len] = 0;
        ESP_LOGI(__func__, "%s", data);
        return ESP_OK;
    }
    default:
        return ESP_OK;
    }
}

extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    vTaskDelay(pdMS_TO_TICKS(5 * 1000));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Xtronic",
            .password = "Om3gaki113r",
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_connect();

    vTaskDelay(pdMS_TO_TICKS(5 * 1000));

    const auto [status, response] = ::Omega::WebServices::GET(URL);
    for (const auto data : response)
    {
        printf("%c", (char)data);
    }
    printf("\r\n");
}
