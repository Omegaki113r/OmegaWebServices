/**
 * @file Response.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:40:26 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Response.hpp
 * File Created: Wednesday, 8th January 2025 12:40:26 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 28th February 2025 1:07:03 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#ifdef __cplusplus
#include <memory>
#endif

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Header.hpp"

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

#ifdef __cplusplus
        struct Data
        {
            u16 status_code;
            Header header;
            u8 *body;
            size_t body_size;

            Data() = default;
            Data(u16 status, const Header &in_header, u8 *in_body) : status_code(status), header(in_header), m_body(std::shared_ptr<u8>(in_body, CHeapDeleter())) { body = m_body.get(); }
            Data(u16 status, const Header &in_header, const std::shared_ptr<u8> in_body) : status_code(status), header(in_header), m_body(in_body) { body = m_body.get(); }
            Data(u16 status, const Header &in_header) : status_code(status), header(in_header) {}

        private:
            std::shared_ptr<u8> m_body;
        };
#else
        struct Data
        {
            Header header;
            u8 *body;
            size_t body_size;
        };
#endif

        struct Response
        {
            const OmegaStatus status;
            Data data;
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