/**
 * @file Request.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:44:32 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Request.hpp
 * File Created: Wednesday, 8th January 2025 12:44:32 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Sunday, 9th February 2025 6:59:10 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaWebServices/Authentication.hpp"
#include "OmegaWebServices/Header.hpp"
#include "OmegaWebServices/Response.hpp"

namespace Omega
{
    namespace WebServices
    {
        namespace Requests
        {
            typedef void (*data_callback_t)(const uint8_t *data, const size_t data_length);

            Response GET(const std::string &);
            Response GET(const char *);
            Response GET(const char *, const char *, const char *);
            Response GET(const char *, const Authentication &);
            Response GET(const char *, const Header &);
            Response GET(const char *, const Header &, const Authentication &);
            Response GET(const char *, data_callback_t);
            Response GET(const char *in_url, const Header &in_header, const Authentication &in_authentication, const data_callback_t in_callback);
        } // namespace Requests
    } // namespace WebServices
} // namespace Omega
