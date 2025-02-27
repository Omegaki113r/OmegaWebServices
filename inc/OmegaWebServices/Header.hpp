/**
 * @file Header.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:36:31 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Header.hpp
 * File Created: Wednesday, 8th January 2025 12:36:31 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 28th February 2025 12:40:17 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "esp_debug_helpers.h"

#include "OmegaUtilityDriver/UtilityDriver.hpp"
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
        class Header
        {
            std::unordered_map<const char *, const char *> header;
            Arena arena{};

        public:
            inline void add_header(const char *in_key, const char *in_value) noexcept
            {
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
            const char *get_value(const char *in_key) noexcept { return header[in_key]; }
            auto begin() const noexcept { return header.begin(); }
            auto end() const noexcept { return header.end(); }

            Header() = default;
            Header(const Header &in_header)
            {
                for (const auto &[key, value] : in_header)
                    add_header(key, value);
            }
            Header &operator=(const Header &in_header)
            {
                for (const auto &[key, value] : in_header)
                    add_header(key, value);
                return *this;
            }
            Header(Header &&) = delete;
            Header &operator=(Header &&) = delete;

            ~Header()
            {
                header.clear();
                arena_free(&arena);
            }
        };
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