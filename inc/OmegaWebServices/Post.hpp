/**
 * @file Post.hpp
 * @author Omegaki113r
 * @date Monday, 3rd March 2025 2:29:14 am
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Post.hpp
 * File Created: Monday, 3rd March 2025 2:29:14 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 8th March 2025 3:44:28 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/RequestBase.hpp"
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
            template <typename T>
            class POST : public RequestBase
            {
                RequsetType m_type;
                char m_url[100]{0};
                char m_host[100]{0};
                u16 m_port;
                char m_path[100]{0};
                Header m_header;
                Authentication m_authentication;
                T m_hardware_base;

                void set_type(RequsetType type) noexcept override { m_type = RequsetType::POST; }
                POST &type(RequsetType type) noexcept override
                {
                    m_type = RequsetType::POST;
                    return *this;
                }

            public:
                POST(T hardware_base) : m_hardware_base(hardware_base) {}
                ~POST() {}

                RequsetType get_type() const noexcept override
                {
                    return RequsetType::POST;
                }

                const char *get_url() const noexcept override
                {
                    return m_url;
                }
                void set_url(const char *url) noexcept override
                {
                    if (url)
                        memcpy(m_url, url, std::strlen(url));
                }
                POST &url(const char *url) noexcept override
                {
                    if (url)
                        memcpy(m_url, url, std::strlen(url));
                    return *this;
                }

                const char *get_host() const noexcept override
                {
                    return m_host;
                }
                void set_host(const char *host) noexcept override
                {
                    if (host)
                        memcpy(m_host, host, std::strlen(host));
                }
                POST &host(const char *host) noexcept override
                {
                    if (host)
                        memcpy(m_host, host, std::strlen(host));
                    return *this;
                }

                u16 get_port() const noexcept override
                {
                    return m_port;
                }
                void set_port(u16 port) noexcept override
                {
                    m_port = port;
                }
                POST &port(u16 port) noexcept override
                {
                    m_port = port;
                    return *this;
                }

                const char *get_path() const noexcept override
                {
                    return m_path;
                }
                void set_path(const char *path) noexcept override
                {
                    if (nullptr != path)
                        memcpy(m_path, path, std::strlen(path));
                }
                POST &path(const char *path) noexcept override
                {
                    if (nullptr != path)
                        memcpy(m_path, path, std::strlen(path));
                    return *this;
                }

                const Header &get_header() const noexcept override
                {
                    return m_header;
                }
                void set_header(const Header &in_header) noexcept override
                {
                    m_header = in_header;
                }
                POST &header(const Header &in_header) noexcept override
                {
                    m_header = in_header;
                    return *this;
                }
                POST &header(const char *key, const char *value) noexcept override
                {
                    m_header.add_header(key, value);
                    return *this;
                }

                const Authentication &get_authentication() const noexcept override
                {
                    return m_authentication;
                }
                void set_authentication(const Authentication &in_auth) noexcept override
                {
                    m_authentication = in_auth;
                }
                POST &authentication(const Authentication &in_auth) noexcept override
                {
                    m_authentication = in_auth;
                    return *this;
                }
                Response perform(std::function<void(const u8 *data, size_t data_length)> chunked_callback = nullptr) noexcept override
                {
                    if (0 < std::strlen(m_url))
                    {
                        return m_hardware_base.perform(Request::RequsetType::POST, m_url, m_authentication, m_header, chunked_callback);
                    }
                    else
                    {
                        return m_hardware_base.perform(Request::RequsetType::POST, m_host, m_port, m_path, m_authentication, m_header, chunked_callback);
                    }
                }
            };
        }
    }
}

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