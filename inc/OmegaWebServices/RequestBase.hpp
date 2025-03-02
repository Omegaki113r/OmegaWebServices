/**
 * @file RequestBase.hpp
 * @author Omegaki113r
 * @date Friday, 14th February 2025 8:23:32 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: RequestBase.hpp
 * File Created: Friday, 14th February 2025 8:23:32 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Sunday, 2nd March 2025 4:23:47 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <cstring>
#include <functional>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/HardwareBase.hpp"
#include "OmegaWebServices/Header.hpp"
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
        namespace Request
        {
            enum class RequsetType
            {
                GET,
                POST
            };

            class RequestBase
            {
            public:
                RequestBase() {}
                ~RequestBase() {}
                virtual RequestBase &type(RequsetType type) noexcept = 0;
                virtual RequestBase &url(const char *url) noexcept = 0;
                virtual RequestBase &host(const char *host) noexcept = 0;
                virtual RequestBase &port(u16 port) noexcept = 0;
                virtual RequestBase &path(const char *path) noexcept = 0;
                virtual RequestBase &header(const Header &header) noexcept = 0;
                virtual RequestBase &authentication(const Authentication &auth) noexcept = 0;

                virtual void set_type(RequsetType type) noexcept = 0;
                virtual void set_url(const char *url) noexcept = 0;
                virtual void set_host(const char *host) noexcept = 0;
                virtual void set_port(u16 port) noexcept = 0;
                virtual void set_path(const char *path) noexcept = 0;
                virtual void set_header(const Header &header) noexcept = 0;
                virtual void set_authentication(const Authentication &auth) noexcept = 0;

                virtual RequsetType get_type() const noexcept = 0;
                virtual const char *get_url() const noexcept = 0;
                virtual const char *get_host() const noexcept = 0;
                virtual u16 get_port() const noexcept = 0;
                virtual const char *get_path() const noexcept = 0;
                virtual const Header &get_header() const noexcept = 0;
                virtual const Authentication &get_authentication() const noexcept = 0;

                virtual Response perform() noexcept = 0;
                virtual Response perform_chunked(std::function<void(const u8 *data, size_t data_length)>) noexcept = 0;
            };
        } // namespace Request
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