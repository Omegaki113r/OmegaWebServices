/**
 * @file Request.cpp
 * @author Omegaki113r
 * @date Wednesday, 15th January 2025 2:22:51 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: Request.cpp
 * File Created: Wednesday, 15th January 2025 2:22:51 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 28th February 2025 12:17:02 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <expected>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <esp_http_client.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/Request.hpp"
#include "OmegaWebServices/Response.hpp"

#include <sdkconfig.h>
#if CONFIG_OMEGA_WEB_SERVICES_DEBUG
#define LOGD(format, ...) OMEGA_LOGD(format, ##__VA_ARGS__)
#define HEX_LOGD(buffer, length) OMEGA_HEX_LOGD(buffer, length)
#else
#define LOGD(format, ...)
#define HEX_LOGD(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_INFO
#define LOGI(format, ...) OMEGA_LOGI(format, ##__VA_ARGS__)
#define HEX_LOGI(buffer, length) OMEGA_HEX_LOGI(buffer, length)
#else
#define LOGI(format, ...)
#define HEX_LOGI(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_WARN
#define LOGW(format, ...) OMEGA_LOGW(format, ##__VA_ARGS__)
#define HEX_LOGW(buffer, length) OMEGA_HEX_LOGW(buffer, length)
#else
#define LOGW(format, ...)
#define HEX_LOGW(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_ERROR
#define LOGE(format, ...) OMEGA_LOGE(format, ##__VA_ARGS__)
#define HEX_LOGE(buffer, length) OMEGA_HEX_LOGE(buffer, length)
#else
#define LOGE(format, ...)
#define HEX_LOGE(buffer, length)
#endif

namespace Omega
{
    namespace WebServices
    {
        namespace Requests
        {
            struct _Response
            {
                const char *m_url;
                const Header &m_header;
                const Authentication &m_auth;
                const data_callback_t &m_callback;
                Response &m_response;
            };

            Response GET(const std::string &in_url) { return GET(in_url.c_str(), {}, {}, nullptr); }

            Response GET(const char *in_url) { return GET(in_url, {}, {}, nullptr); }

            Response GET(const char *in_url, const char *in_username, const char *in_password) { return GET(in_url, {}, {in_username, in_password}, nullptr); }

            Response GET(const char *in_url, const Authentication &in_authentication) { return GET(in_url, {}, in_authentication, nullptr); }

            Response GET(const char *in_url, const Header &in_header) { return GET(in_url, in_header, {}, nullptr); }

            Response GET(const char *in_url, data_callback_t callback) { return GET(in_url, {}, {}, callback); }

            Response GET(const char *in_url, const Header &in_header, const Authentication &in_authentication, const data_callback_t in_callback)
            {
                if (nullptr == in_url || 0 == std::strlen(in_url))
                {
                    LOGE("Provided URL is invalid");
                    return {eFAILED};
                }
                const auto http_event_handler = [](esp_http_client_event_t *evt)
                {
                    auto response = static_cast<_Response *>(evt->user_data);
                    switch (evt->event_id)
                    {
                    case HTTP_EVENT_ERROR:
                    {
                        LOGE("HTTP_EVENT_ERROR");
                        break;
                    }
                    case HTTP_EVENT_ON_CONNECTED:
                    {
                        LOGD("HTTP_EVENT_ON_CONNECTED");
                        break;
                    }
                    case HTTP_EVENT_HEADER_SENT:
                    {
                        LOGD("HTTP_EVENT_HEADERS_SENT/HTTP_EVENT_HEADER_SENT");
                        break;
                    }
                    case HTTP_EVENT_ON_HEADER:
                    {
                        LOGD("HTTP_EVENT_ON_HEADER");
                        response->m_response.data.header.add_header(evt->header_key, evt->header_value);
                        break;
                    }
                    case HTTP_EVENT_ON_DATA:
                    {
                        LOGD("HTTP_EVENT_ON_DATA");
                        const auto *data = static_cast<const u8 *>(evt->data);
                        const auto data_len = evt->data_len;
                        if (esp_http_client_is_chunked_response(evt->client))
                        {
                            if (nullptr != response->m_callback)
                                response->m_callback(data, data_len);
                        }
                        else
                        {
                            response->m_response.data.body = static_cast<u8 *>(omega_malloc(sizeof(u8) * data_len));
                            if (nullptr == response->m_response.data.body)
                            {
                                LOGE("Allocating memory failed for body");
                                return ESP_FAIL;
                            }
                            UNUSED(std::memcpy(response->m_response.data.body, data, data_len));
                        }
                        break;
                    }
                    case HTTP_EVENT_ON_FINISH:
                    {
                        LOGD("HTTP_EVENT_ON_FINISH");
                        break;
                    }
                    case HTTP_EVENT_DISCONNECTED:
                    {
                        LOGD("HTTP_EVENT_DISCONNECTED");
                        break;
                    }
                    case HTTP_EVENT_REDIRECT:
                    {
                        LOGD("HTTP_EVENT_REDIRECT");
                        break;
                    }
                    default:
                    {
                        LOGE("Unhandled event: %d", evt->event_id);
                        break;
                    }
                    }
                    return ESP_OK;
                };
                Response response{};
                _Response _response{in_url, in_header, in_authentication, in_callback, response};
                const esp_http_client_config_t http_config{
                    .url = in_url,
                    .username = in_authentication.username,
                    .password = in_authentication.password,
                    .method = HTTP_METHOD_GET,
                    .event_handler = http_event_handler,
                    .user_data = &_response,
                };
                esp_http_client_handle_t handle = esp_http_client_init(&http_config);
                for (const auto &[key, value] : in_header)
                {
                    LOGE("esp_http_client_set_header failed for [%s]:%s", key, value);
                    // if (ESP_OK != esp_http_client_set_header(handle, key, value))
                    // {
                    //     LOGE("esp_http_client_set_header failed for [%s]:%s", key.c_str(), value.c_str());
                    //     return {eFAILED};
                    // }
                }
                if (ESP_OK != esp_http_client_perform(handle))
                {
                    LOGE("esp_http_client_perform failed");
                    return {eFAILED};
                }
                if (ESP_OK != esp_http_client_cleanup(handle))
                {
                    LOGE("esp_http_client_cleanup failed");
                    return {eFAILED};
                }
                return _response.m_response;
            }

            Response POST(const char *in_url, const Header &in_header, const Authentication &in_authentication, const data_callback_t in_callback)
            {
                if (nullptr == in_url || 0 == std::strlen(in_url))
                {
                    LOGE("Provided URL is invalid");
                    return {eFAILED};
                }
                const auto http_event_handler = [](esp_http_client_event_t *evt)
                {
                    auto response = static_cast<_Response *>(evt->user_data);
                    switch (evt->event_id)
                    {
                    case HTTP_EVENT_ERROR:
                    {
                        LOGE("HTTP_EVENT_ERROR");
                        break;
                    }
                    case HTTP_EVENT_ON_CONNECTED:
                    {
                        LOGD("HTTP_EVENT_ON_CONNECTED");
                        break;
                    }
                    case HTTP_EVENT_HEADER_SENT:
                    {
                        LOGD("HTTP_EVENT_HEADERS_SENT/HTTP_EVENT_HEADER_SENT");
                        break;
                    }
                    case HTTP_EVENT_ON_HEADER:
                    {
                        LOGD("HTTP_EVENT_ON_HEADER");
                        response->m_response.data.header.add_header(evt->header_key, evt->header_value);
                        break;
                    }
                    case HTTP_EVENT_ON_DATA:
                    {
                        LOGD("HTTP_EVENT_ON_DATA");
                        const auto *data = static_cast<const u8 *>(evt->data);
                        const auto data_len = evt->data_len;
                        if (esp_http_client_is_chunked_response(evt->client))
                        {
                            if (nullptr != response->m_callback)
                                response->m_callback(data, data_len);
                        }
                        else
                        {
                            response->m_response.data.body = static_cast<u8 *>(omega_malloc(sizeof(u8) * data_len));
                            if (nullptr == response->m_response.data.body)
                            {
                                LOGE("Allocating memory failed for body");
                                return ESP_FAIL;
                            }
                            UNUSED(std::memcpy(response->m_response.data.body, data, data_len));
                        }
                        break;
                    }
                    case HTTP_EVENT_ON_FINISH:
                    {
                        LOGD("HTTP_EVENT_ON_FINISH");
                        break;
                    }
                    case HTTP_EVENT_DISCONNECTED:
                    {
                        LOGD("HTTP_EVENT_DISCONNECTED");
                        break;
                    }
                    case HTTP_EVENT_REDIRECT:
                    {
                        LOGD("HTTP_EVENT_REDIRECT");
                        break;
                    }
                    default:
                    {
                        LOGE("Unhandled event: %d", evt->event_id);
                        break;
                    }
                    }
                    return ESP_OK;
                };
                Response response{};
                _Response _response{in_url, in_header, in_authentication, in_callback, response};
                const esp_http_client_config_t http_config{
                    .url = in_url,
                    .username = in_authentication.username,
                    .password = in_authentication.password,
                    .method = HTTP_METHOD_POST,
                    .event_handler = http_event_handler,
                    .user_data = &_response,
                };
                esp_http_client_handle_t handle = esp_http_client_init(&http_config);
                if (ESP_OK != esp_http_client_perform(handle))
                {
                    LOGE("esp_http_client_perform failed");
                    return {eFAILED};
                }
                if (ESP_OK != esp_http_client_cleanup(handle))
                {
                    LOGE("esp_http_client_cleanup failed");
                    return {eFAILED};
                }
                return _response.m_response;
            }
        } // namespace Requests
    } // namespace WebServices
} // namespace Omega
