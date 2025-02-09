/**
 * @file MQTT.hpp
 * @author Omegaki113r
 * @date Sunday, 9th February 2025 7:00:28 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: MQTT.hpp
 * File Created: Sunday, 9th February 2025 7:00:28 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Sunday, 9th February 2025 8:51:33 pm
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

namespace Omega
{
    namespace WebServices
    {
        namespace MQTT
        {
            namespace CLIENT
            {
                OmegaStatus connect(const char *in_url, u16 in_port, const char *in_username, const char *in_password) noexcept;
                OmegaStatus connect(const char *in_url, u16 in_port) noexcept;
                void publish(const char *in_topic, const u8 *in_data, size_t in_data_length, u8 qos = 0) noexcept;
                void publish(const char *in_topic, const char *in_data, size_t in_data_length, u8 qos = 0) noexcept;
            } // namespace CLIENT
        } // namespace MQTT
    } // namespace WebServices
} // namespace Omega
