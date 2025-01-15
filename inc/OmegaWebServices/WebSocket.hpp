/**
 * @file WebSocket.hpp
 * @author Omegaki113r
 * @date Wednesday, 15th January 2025 2:30:10 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: WebSocket.hpp
 * File Created: Wednesday, 15th January 2025 2:30:10 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 16th January 2025 1:48:24 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <string>

#include "OmegaWebServices/Authentication.hpp"

namespace Omega
{
    namespace WebServices
    {
        namespace WebSocket
        {
            namespace Client
            {
                OmegaStatus Init(const char *in_url);
                OmegaStatus Init(const char *in_url, const Authentication &in_auth);
                OmegaStatus Init(const char *in_url, const char *in_username, const char *in_password);
                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len);
            } // namespace Client

            namespace Server
            {
                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len);
            } // namespace Server
        } // namespace WebSocketClient
    } // namespace WebServices
} // namespace Omega
