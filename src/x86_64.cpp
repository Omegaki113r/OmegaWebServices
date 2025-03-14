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
 * Last Modified: Friday, 14th March 2025 6:20:25 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <functional>
#include <memory>

// #include <Poco/Exception.h>
// #include <Poco/Net/HTMLForm.h>
// #include <Poco/Net/HTTPClientSession.h>
// #include <Poco/Net/HTTPRequest.h>
// #include <Poco/Net/HTTPResponse.h>
// #include <Poco/StreamCopier.h>

#include <mqtt/async_client.h>

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

        class callback : public virtual mqtt::callback
        {

        public:
            std::function<void(void)> m_on_connected;
            std::function<void(const u8 *, size_t)> m_on_data;
            std::function<void(void)> m_on_disconnected;

            void set_callback(std::function<void(void)> on_connected, std::function<void(const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) {
                m_on_connected = on_connected; 
                m_on_data = on_data;  
                m_on_disconnected = on_disconnected;
            }

            void connected(const std::string &cause) override
            {
                LOGD("Connected: %s", cause.c_str());
                m_on_connected();
            }

            void message_arrived(mqtt::const_message_ptr msg) override
            {
                LOGD("Received: %s: %s", msg->get_topic(), msg->get_payload_str());
            }

            void connection_lost(const std::string &cause) override
            {
                LOGD("Disconnected: %s", cause.c_str());
                m_on_disconnected();
            }
        };

        const std::string SERVER_ADDRESS{"192.168.43.159:1883"};
        const std::string CLIENT_ID{"paho_cpp_client"};
        const std::string TOPIC{"test/topic"};

        mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
        mqtt::connect_options connOpts;
       callback cb;

        OmegaStatus x86_64::connect_mqtt(const char *url, const Authentication &auth, std::function<void(void)> on_connected, std::function<void(const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept
        {
            cb.set_callback(on_connected, on_data, on_disconnected);
            client.set_callback(cb);
            connOpts.set_keep_alive_interval(20);
            connOpts.set_clean_session(true);
            try
            {
                client.connect(connOpts)->wait();
            //     std::cout << "Connecting to " << SERVER_ADDRESS << "..." << std::endl;
            //     std::cout << "Subscribing to " << TOPIC << "..." << std::endl;
            //     client.subscribe(TOPIC, 1)->wait();
            //     std::cout << "Publishing message..." << std::endl;
            //     client.publish(TOPIC, "Hello from Paho C++!", 1, false)->wait();
            //     std::this_thread::sleep_for(std::chrono::seconds(10));
            //     std::cout << "Disconnecting..." << std::endl;
            //     client.disconnect()->wait();
            }
            catch (const mqtt::exception &exc)
            {
                std::cerr << "Error: " << exc.what() << std::endl;
                return eFAILED;
            }
            return eSUCCESS;
        }

        OmegaStatus x86_64::connect_mqtt(const char *host, u16 port, const Authentication &auth, std::function<void(void)> on_connected, std::function<void(const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept { return eFAILED; }

        OmegaStatus x86_64::disconnect_mqtt(std::function<void(void)> on_disconnected) noexcept
        {
            client.disconnect(0);
            on_disconnected();
            return eSUCCESS;
        }
    } // namespace WebServices
} // namespace Omega