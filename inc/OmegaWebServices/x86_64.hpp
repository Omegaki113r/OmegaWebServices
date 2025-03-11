/**
 * @file x86_64.hpp
 * @author Omegaki113r
 * @date Tuesday, 11th March 2025 6:55:05 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: x86_64.hpp
 * File Created: Tuesday, 11th March 2025 6:55:05 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 11th March 2025 8:33:56 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <functional>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/HardwareBase.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/Response.hpp"

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
        class x86_64 : public HardwareBase
        {
            Response perform_get(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback = nullptr) noexcept;
            Response perform_post(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback = nullptr) noexcept;
            Response stream_post(const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback = nullptr) noexcept;

        public:
            Response perform(Request::RequsetType type,
                             const char *url, const Authentication &auth, const Header &header,
                             std::function<void(const u8 *data, size_t data_length)> chunked_callback = nullptr) noexcept override;
            Response perform(Request::RequsetType type,
                             const char *host, u16 port, const char *path, const Authentication &auth, const Header &header,
                             std::function<void(const u8 *data, size_t data_length)> chunked_callback = nullptr) noexcept override;
            Response stream(Request::RequsetType type,
                            const char *url, const Authentication &auth, const Header &header,
                            std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept override;
            Response stream(Request::RequsetType type,
                            const char *host, u16 port, const char *path, const Authentication &auth, const Header &header,
                            std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept override;
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