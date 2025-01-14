/**
 * @file GET.cpp
 * @author Omegaki113r
 * @date Sunday, 5th January 2025 1:08:05 am
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: test
 * File Name: GET.cpp
 * File Created: Sunday, 5th January 2025 1:08:05 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 7th January 2025 12:10:36 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <nvs.h>
#include <nvs_flash.h>

#include <unity.h>

#include "OmegaWebServices/WebServices.hpp"
#include "OmegaWiFiController.hpp"

TEST_CASE("OmegaWiFiController", "[GET]")
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ::Omega::WiFiController::initialize(::Omega::WiFiController::Mode::eSTATION_MODE);
    ::Omega::WiFiController::connect("Xtronic", "Om3gaki113r");
    ::Omega::WiFiController::wait_for_connection();

    delay(5_s);
    const auto [status, response] = ::Omega::WebServices::GET("");
    TEST_ASSERT_EQUAL(status, eSUCCESS);
}