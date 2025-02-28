/**
 * @file HardwareBase.hpp
 * @author Omegaki113r
 * @date Friday, 21st February 2025 4:31:43 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: HardwareBase.hpp
 * File Created: Friday, 21st February 2025 4:31:43 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 1st March 2025 5:13:46 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
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
#include "OmegaWebServices/Response.hpp"

namespace Omega
{
    namespace WebServices
    {
        class HardwareBase
        {
        public:
            virtual Response perform(const char *url, const Authentication &auth, const Header &header) noexcept = 0;
            virtual Response perform_chunked(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept = 0;
            virtual Response perform(const char *host, u16 port, const char *path, const Authentication &auth, const Header &header) noexcept = 0;
            virtual Response perform_chunked(const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept = 0;
        };
    } // namespace WebServices
} // namespace Omega