/**
 * @file Request.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:44:32 am
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Request.hpp
 * File Created: Wednesday, 8th January 2025 12:44:32 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 8th January 2025 12:45:57 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaWebServices/Authenticate.hpp"
#include "OmegaWebServices/Header.hpp"

namespace Omega
{
    namespace WebServices
    {
        struct Request
        {
            Authentication auth;
            Header header;
        };
    } // namespace WebServices
} // namespace Omega
