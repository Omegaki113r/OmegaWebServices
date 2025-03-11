/**
 * @file x86_64.cpp
 * @author Omegaki113r
 * @date Tuesday, 11th March 2025 6:56:11 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: x86_64.cpp
 * File Created: Tuesday, 11th March 2025 6:56:11 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 12th March 2025 1:55:40 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <memory>

// #include <Poco/Exception.h>
// #include <Poco/Net/HTMLForm.h>
// #include <Poco/Net/HTTPClientSession.h>
// #include <Poco/Net/HTTPRequest.h>
// #include <Poco/Net/HTTPResponse.h>
// #include <Poco/StreamCopier.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/x86_64.hpp"

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

namespace Omega
{
    namespace WebServices
    {
        Response x86_64::perform_get(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            // using namespace Poco::Net;
            // HTTPClientSession s("www.somehost.com");
            // // s.setProxy("localhost", srv.port());
            // HTTPRequest request(HTTPRequest::HTTP_GET, "/large");
            // HTMLForm form;
            // form.add("entry1", "value1");
            // form.prepareSubmit(request);
            // s.sendRequest(request);
            // HTTPResponse response;
            // std::istream &rs = s.receiveResponse(response);

            // Poco::StreamCopier::copyStream(rs, std::cout);
            return {eFAILED, {}};
        }

        Response x86_64::perform_post(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            return {eFAILED, {}};
        }

        Response x86_64::stream_post(const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> stream_callback) noexcept
        {
            return {eFAILED, {}};
        }

        Response x86_64::perform(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            if (Request::RequsetType::GET == type)
            {
                return perform_get(url, auth, header, chunked_callback);
            }
            else if (Request::RequsetType::POST == type)
            {
                return perform_post(url, auth, header, chunked_callback);
            }
            return {eFAILED, {}};
        }

        Response x86_64::perform(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

        Response x86_64::stream(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
        {
            if (Request::RequsetType::GET == type)
            {
                LOGE("NOT IMPLEMENTED");
                return {eFAILED, {}};
            }
            else if (Request::RequsetType::POST == type)
            {
                return stream_post(url, auth, header, chunked_callback);
            }
            return {eFAILED, {}};
        }

        Response x86_64::stream(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }
    } // namespace WebServices
} // namespace Omega