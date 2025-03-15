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
 * Last Modified: Sunday, 16th March 2025 12:58:04 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

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

        Response x86_64::perform_request(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
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

        Response x86_64::perform_request(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

        Response x86_64::stream_request(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
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

        Response x86_64::stream_request(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

        OmegaStatus x86_64::connect_mqtt(const char *url, const Authentication &auth, const char *client_id, std::function<void(void)> on_connected, std::function<void(const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept
        {
            MQTTClient mqtt_handle{};
            MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
            MQTTClient_message pubmsg = MQTTClient_message_initializer;
            MQTTClient_deliveryToken token;

            m_handlers = { on_connected, on_data, on_disconnected};
            if (const auto state = MQTTClient_create(&mqtt_handle, url, client_id, MQTTCLIENT_PERSISTENCE_NONE, nullptr); MQTTCLIENT_SUCCESS != state) {
                OMEGA_LOGE("MQTTClient_create failed with %s", MQTTClient_strerror(state));
                return eFAILED;
            }

            const auto on_connection_handler = [](void* context, char* cause) 
            {
                    LOGD("Connection handler called");
            };
            const auto on_message_arrived_handler = [](void* context, char* topicName, int topicLen, MQTTClient_message* message) -> int
            {
                    LOGD("%s: %s", topicName, message->payload);
                    MQTTClient_free(topicName);
                    MQTTClient_freeMessage(&message);
                    return 1;
            };
            const auto on_message_delivered_handler = [](void* context, MQTTClient_deliveryToken dt)
            {
                    LOGD("Message delivered");
            };

            if (const auto state = MQTTClient_setCallbacks(mqtt_handle, &m_handlers, on_connection_handler, on_message_arrived_handler, on_message_delivered_handler); MQTTCLIENT_SUCCESS != state)
            {
                LOGE("MQTTClient_create failed with %s", MQTTClient_strerror(state));
                return eFAILED;
            }

            if (const auto state = MQTTClient_connect(mqtt_handle, &conn_opts); MQTTCLIENT_SUCCESS != state) 
            {
                LOGE("MQTTClient_connect with %s", MQTTClient_strerror(state));
                return eFAILED;
            }

            size_t attempt = 0;
            while(!MQTTClient_isConnected(mqtt_handle))
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                attempt++;
                if (attempt > 10) {
                    break;
                }
            }
            if(MQTTClient_isConnected(mqtt_handle))
            {
                m_connection = mqtt_handle;
                on_connected();
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus x86_64::connect_mqtt(const char *host, u16 port, const Authentication &auth, const char *client_id, std::function<void(void)> on_connected, std::function<void(const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept { return eFAILED; }

        OmegaStatus x86_64::subscribe_mqtt(const char *topic, u8 qos) noexcept
        {
            if (const auto state = MQTTClient_subscribe(std::get<MQTTClient>(m_connection), topic, qos);MQTTCLIENT_SUCCESS != state)
            {
                LOGE("MQTTClient_subscribe failed to :%s with %s", topic, MQTTClient_strerror(state));
                return eFAILED;
            }
            return eSUCCESS;
        }

        OmegaStatus x86_64::publish_mqtt(const char *topic, const u8 *data, size_t data_length, u8 qos) noexcept
        {
            if (const auto err = MQTTClient_publish(std::get<MQTTClient>(m_connection), topic, data_length, data, qos, false, nullptr);MQTTCLIENT_SUCCESS != err) {
                LOGE("MQTTClient_publish failed with: %s", MQTTClient_strerror(err));
                return eFAILED;
            }
            return eFAILED;
        }

        OmegaStatus x86_64::disconnect_mqtt(std::function<void(void)> on_disconnected) noexcept
        {
            if (const auto err = MQTTClient_disconnect(std::get<MQTTClient>(m_connection), 5 * 1000);MQTTCLIENT_SUCCESS != err) {
                LOGE("MQTTClient_disconnect failed with :%s", MQTTClient_strerror(err));
                return eFAILED;
            }
            on_disconnected();
            return eSUCCESS;
        }

    } // namespace WebServices
} // namespace Omega