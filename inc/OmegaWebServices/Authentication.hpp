/**
 * @file Authentication.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:39:16 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Authentication.hpp
 * File Created: Wednesday, 8th January 2025 12:39:16 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 14th January 2025 11:19:43 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <cstring>

namespace Omega
{
    namespace WebServices
    {
        constexpr size_t USERNAME_BUFFER_SIZE = 100;
        constexpr size_t PASSWORD_BUFFER_SIZE = 100;

        struct Authentication
        {
            char username[USERNAME_BUFFER_SIZE + 1];
            char password[PASSWORD_BUFFER_SIZE + 1];

            Authentication() = default;
            Authentication(const char *in_username, const char *in_password)
            {
                if (nullptr != in_username && 0 < std::strlen(in_username))
                    std::strncpy(username, in_username, std::strlen(in_username));
                if (nullptr != in_password && 0 < std::strlen(in_password))
                    std::strncpy(password, in_password, std::strlen(in_password));
            }
        };
    } // namespace WebServices
} // namespace Omega
