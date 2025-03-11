/**
 * @file Header.cpp
 * @author Omegaki113r
 * @date Tuesday, 11th March 2025 7:16:11 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: Header.cpp
 * File Created: Tuesday, 11th March 2025 7:16:11 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 11th March 2025 7:22:03 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <cstring>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Header.hpp"

#define ARENA_IMPLEMENTATION
#include "OmegaUtilityDriver/arena.h"

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
        void Header::add_header(const char *in_key, const char *in_value) noexcept
        {
            if (nullptr == in_key || nullptr == in_value)
            {
                LOGE("Invalid input parameters");
                return;
            }
            if (0 == std::strlen(in_key) || 0 == std::strlen(in_value))
            {
                LOGE("Invalid input parameter length");
                return;
            }
            const size_t key_length = std::strlen(in_key);
            char *key = static_cast<char *>(arena_alloc(&arena, key_length + 1));
            UNUSED(std::memcpy(key, in_key, key_length));
            key[key_length] = '\0';
            const size_t value_length = std::strlen(in_value);
            char *value = static_cast<char *>(arena_alloc(&arena, value_length + 1));
            UNUSED(std::memcpy(value, in_value, value_length));
            value[value_length] = '\0';
            header[key] = value;
        }
    } // namespace WebServices
} // namespace Omega