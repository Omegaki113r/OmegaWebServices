#include <cstring>
#include <stdio.h>
#include <variant>
#include <vector>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/x86_64.hpp"
#include "OmegaWebServices/Get.hpp"

 #define URL "https://httpbin.org/get"
// #define URL "https://httpbin.org/stream/1"
// #define URL "https://randomuser.me/api/"
//#define URL "http://192.168.43.159:3000/ota"
// #define URL "http://127.0.0.1:3000/uploads/"
#define URL_LEN std::strlen(URL)

#define URL_IMG "https://randomuser.me/api/portraits/thumb/women/27.jpg"
#define URL_IMG_LEN std::strlen(URL_IMG)

int main(void)
{

    //// const auto start_time = esp_timer_get_time();
    const auto chunked_callback = [&](const u8 *data, size_t data_length)
    {
        // OMEGA_LOGW("Chunk received with length: %d", data_length);
        // OMEGA_HEX_LOGI((void *)data, data_length);
    };

    
        auto [status, data] = ::Omega::WebServices::Request::GET(::Omega::WebServices::x86_64())
            .url(URL)
            .perform(chunked_callback);
        for (const auto& [key, value] : data.header)
        {
            OMEGA_LOGI("%s: %s", key, value);
        }

    // OMEGA_LOGI("[%.1f MB] execution time: %.1fs",
    //            static_cast<float>(data.body_size) / (1000.0f * 1000.0f),
    //            (static_cast<float>(esp_timer_get_time()) - static_cast<float>(start_time)) / (1000.0f * 1000.0f));

    return 0;
}