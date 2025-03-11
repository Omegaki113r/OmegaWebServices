#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string.h>

#define DNS_PORT 53
#define HTTP_PORT 80
#define IP_ADDRESS "192.168.4.1" // IP address of ESP32 in AP mode

static const char *TAG = "DNS_HTTP_SERVER";

// Function to initialize Wi-Fi in AP mode
static void wifi_init_ap(void)
{
    esp_netif_init();                // Initialize the network interface
    esp_event_loop_create_default(); // Create default event loop

    // Initialize Wi-Fi in AP mode
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_netif_create_default_wifi_ap(); // Create default AP interface

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ESP32_DNS", // SSID for the AP
            .ssid_len = strlen("ESP32_DNS"),
            .password = "123456789", // Password for the AP
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK},
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Wi-Fi AP started. SSID: ESP32_DNS, Password: 123456789");
}

// Function to log DNS query types
static const char *get_query_type(uint16_t query_type)
{
    switch (query_type)
    {
    case 0x0001:
        return "A (IPv4)";
    case 0x001c:
        return "AAAA (IPv6)";
    case 0x000f:
        return "MX (Mail Exchange)";
    case 0x0005:
        return "CNAME (Canonical Name)";
    case 0x000c:
        return "PTR (Pointer)";
    case 0x0020:
        return "NS (Name Server)";
    default:
        return "Unknown Type";
    }
}

// Function to handle DNS requests
static void dns_server_task(void *pvParameters)
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char recv_buffer[512];
    int sockfd, recv_len;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket");
        vTaskDelete(NULL);
    }

    // Setup server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the DNS server port (53)
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        ESP_LOGE(TAG, "Bind failed");
        close(sockfd);
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "DNS server listening on port %d", DNS_PORT);

    while (1)
    {
        // Receive a DNS request
        recv_len = recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0,
                            (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0)
        {
            ESP_LOGE(TAG, "Failed to receive DNS query");
            continue;
        }

        ESP_LOGI(TAG, "Received DNS query");

        // Extract the domain name from the request (starting at byte 12)
        char *domain_name = (char *)(recv_buffer + 12);
        ESP_LOGI(TAG, "Requested domain: %s", domain_name);

        // Check if the domain is "captive.apple.com"
        if (strcmp(domain_name, "captiveapplecom") == 0)
        {
            // Create a DNS response with the ESP32's IP address
            uint8_t response[512] = {0};

            // Copy DNS header
            memcpy(response, recv_buffer, 12); // Copy first 12 bytes (DNS header)

            // Set response flags (set as authoritative answer)
            response[2] = 0x81; // Response (0x81) and No Error (0x00)
            response[3] = 0x80;
            response[6] = 0x00; // No additional records

            // Set answer section (1 record)
            int query_len = recv_buffer[12] + recv_buffer[13] + 2; // DNS query length
            int offset = 12 + query_len;                           // Find the answer section

            // Set the answer section: the query name (e.g., any domain queried)
            response[offset] = 0x00;                                        // Name length (for simplicity, 0 for now)
            memcpy(response + offset + 1, recv_buffer + 12, query_len - 1); // Copy the domain name

            // Set type A record (IPv4 address)
            response[offset + query_len] = 0x00;
            response[offset + query_len + 1] = 0x01; // Type A

            // Set class IN (Internet)
            response[offset + query_len + 2] = 0x00;
            response[offset + query_len + 3] = 0x01;

            // Set TTL (time to live)
            response[offset + query_len + 4] = 0x00;
            response[offset + query_len + 5] = 0x00;
            response[offset + query_len + 6] = 0x00;
            response[offset + query_len + 7] = 0x3c; // TTL = 60 seconds

            // Set address length (4 bytes for IPv4)
            response[offset + query_len + 8] = 0x00;
            response[offset + query_len + 9] = 0x04;

            // Set the IP address (example: 192.168.4.1, which is the AP IP)
            response[offset + query_len + 10] = 192;
            response[offset + query_len + 11] = 168;
            response[offset + query_len + 12] = 1;
            response[offset + query_len + 13] = 1;

            // Send DNS response
            if (sendto(sockfd, response, sizeof(response), 0,
                       (struct sockaddr *)&client_addr, client_len) < 0)
            {
                ESP_LOGE(TAG, "Failed to send DNS response");
            }
            else
            {
                ESP_LOGI(TAG, "Sent DNS response for domain: %s", domain_name);
            }
        }
        else
        {
            // For other domains, continue to send normal DNS responses (to be handled)
            ESP_LOGI(TAG, "Unknown domain request: %s", domain_name);
        }
    }

    close(sockfd);
    vTaskDelete(NULL);
}

// HTTP handler to serve a simple webpage
esp_err_t http_get_handler(httpd_req_t *req)
{
    const char *resp_str = "<html><body><h1>Welcome to the ESP32 Captive Portal!</h1><p>This is a simple webpage hosted on the ESP32.</p></body></html>";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

// Function to start the HTTP server
void start_http_server()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Register URI handler
    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = http_get_handler,
        .user_ctx = NULL};

    // Start the HTTP server
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        ESP_LOGI(TAG, "HTTP server started on port %d", HTTP_PORT);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to start HTTP server");
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting DNS and HTTP server");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi in AP mode
    wifi_init_ap();

    // Create DNS server task
    xTaskCreate(dns_server_task, "dns_server_task", 4096, NULL, 5, NULL);

    // Start HTTP server
    start_http_server();
}
