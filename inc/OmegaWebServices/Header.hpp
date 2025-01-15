/**
 * @file Header.hpp
 * @author Omegaki113r
 * @date Wednesday, 8th January 2025 12:36:31 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: Header.hpp
 * File Created: Wednesday, 8th January 2025 12:36:31 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 15th January 2025 1:14:30 am
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
#include <unordered_map>

namespace Omega
{
    namespace WebServices
    {
        class Header
        {
            std::unordered_map<std::string, std::string> header;

        public:
            inline void add_header(const char *in_key, const char *in_value) noexcept
            {
                header[in_key] = in_value;
            }
            inline void add_header(const std::string &in_key, const std::string &in_value) noexcept { add_header(in_key.c_str(), in_value.c_str()); }
            const std::string &get_value(const char *in_key) noexcept { return header[in_key]; }
            const std::string &get_value(const std::string &in_key) noexcept { return get_value(in_key.c_str()); }
            auto begin() const noexcept { return header.begin(); }
            auto end() const noexcept { return header.end(); }
        };
    } // namespace WebServices
} // namespace Omega