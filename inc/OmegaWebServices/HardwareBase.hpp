/**
 * @file HardwareBase.hpp
 * @author Omegaki113r
 * @date Friday, 21st February 2025 4:31:43 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: HardwareBase.hpp
 * File Created: Friday, 21st February 2025 4:31:43 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 15th March 2025 5:35:53 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <functional>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/RequestBase.hpp"
#include "OmegaWebServices/Response.hpp"

namespace Omega
{
    namespace WebServices
    {
        class HardwareBase
        {
        public:
            virtual Response perform_request(Request::RequsetType type,
                                             const char *url, const Authentication &auth, const Header &header,
                                             std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept = 0;
            virtual Response perform_request(Request::RequsetType type,
                                             const char *host, u16 port, const char *path, const Authentication &auth, const Header &header,
                                             std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept = 0;
            virtual Response stream_request(Request::RequsetType type,
                                            const char *url, const Authentication &auth, const Header &header,
                                            std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept = 0;
            virtual Response stream_request(Request::RequsetType type,
                                            const char *host, u16 port, const char *path, const Authentication &auth, const Header &header,
                                            std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept = 0;
            virtual OmegaStatus connect_mqtt(const char* url, const Authentication& auth, std::function<void(void)> on_connected, std::function<void(const u8*, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept = 0;
            virtual OmegaStatus connect_mqtt(const char* host, u16 port, const Authentication& auth, std::function<void(void)> on_connected, std::function<void(const u8*, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept = 0;
            virtual OmegaStatus publish_mqtt(const char* topic, const u8* data, size_t data_length, u8 qos) noexcept = 0;
            virtual OmegaStatus subscribe_mqtt(const char* topic, u8 qos) noexcept = 0;
            virtual OmegaStatus disconnect_mqtt(std::function<void(void)> on_disconnected) noexcept = 0;
        };
    } // namespace WebServices
} // namespace Omega