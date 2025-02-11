/**
 * @file WebSocket.hpp
 * @author Omegaki113r
 * @date Wednesday, 15th January 2025 2:30:10 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: WebSocket.hpp
 * File Created: Wednesday, 15th January 2025 2:30:10 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 11th February 2025 2:11:41 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <string>

#include "OmegaWebServices/Authentication.hpp"

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
        namespace WebSocket
        {
            namespace Client
            {
                OmegaStatus Init(const char *in_url);
                OmegaStatus Init(const char *in_url, const Authentication &in_auth);
                OmegaStatus Init(const char *in_url, const char *in_username, const char *in_password);
                OmegaStatus send(const char *path, const char *data, size_t data_length);
                bool is_connected();
                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len);
            } // namespace Client

            namespace Server
            {
                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len);
            } // namespace Server
        } // namespace WebSocketClient
    } // namespace WebServices
} // namespace Omega

#if CONFIG_OMEGA_WEB_SERVICES_DEBUG
#undef LOGD
#undef HEX_LOGD
#endif

#if CONFIG_OMEGA_WEB_SERVICES_INFO
#undef LOGI
#undef HEX_LOGI
#endif

#if CONFIG_OMEGA_WEB_SERVICES_WARN
#undef LOGW
#undef HEX_LOGW
#endif

#if CONFIG_OMEGA_WEB_SERVICES_ERROR
#undef LOGE
#undef HEX_LOGE
#endif