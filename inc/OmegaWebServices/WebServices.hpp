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
 * Last Modified: Friday, 3rd January 2025 8:13:25 pm
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

#include "OmegaUtilityDriver.hpp"

namespace Omega
{
    namespace WebServices
    {
        struct Response
        {
            const OmegaStatus status;
            const std::vector<u8> data;
        };

        Response GET(const std::string &);
        Response GET(const char *);
    } // namespace WebServices
} // namespace Omega
