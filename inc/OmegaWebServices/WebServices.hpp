/**
 * @file WebServices.hpp
 * @author Omegaki113r
 * @date Thursday, 2nd January 2025 8:01:10 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: WebServices.hpp
 * File Created: Thursday, 2nd January 2025 8:01:10 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 15th January 2025 12:58:32 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "OmegaUtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/Response.hpp"

namespace Omega
{
    namespace WebServices
    {
        typedef void (*data_callback_t)(const uint8_t *, const size_t);

        Response GET(const std::string &);
        Response GET(const char *);
        Response GET(const char *, const char *, const char *);
        Response GET(const char *, const Authentication &);
        Response GET(const char *, const Header &);
        Response GET(const char *, const Header &, const Authentication &);
        Response GET(const char *, data_callback_t);
        Response GET(const char *in_url, const Header &in_header, const Authentication &in_authentication, data_callback_t);
    } // namespace WebServices
} // namespace Omega
