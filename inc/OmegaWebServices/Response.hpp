/**
 * @file Response.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:40:26 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Response.hpp
 * File Created: Wednesday, 8th January 2025 12:40:26 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 15th January 2025 1:22:54 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaUtilityDriver.hpp"
#include "OmegaWebServices/Header.hpp"

namespace Omega
{
    namespace WebServices
    {
        struct Data
        {
            Header header;
            u8 *body;
            size_t body_size;
        };

        struct Response
        {
            const OmegaStatus status;
            Data data;
        };
    } // namespace WebServices
} // namespace Omega
