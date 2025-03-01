# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set( WEB_SERVICES_PATH ../.. )
set( EXTRA_COMPONENT_DIRS ${WEB_SERVICES_PATH} )

add_compile_definitions(CONFIG_OMEGA_LOGGING=1)
# add_compile_definitions(CONFIG_OMEGA_WIFI_CONTROLLER_DEBUG=1)
# add_compile_definitions(CONFIG_OMEGA_WIFI_CONTROLLER_INFO=1)
# add_compile_definitions(CONFIG_OMEGA_WIFI_CONTROLLER_WARN=1)
# add_compile_definitions(CONFIG_OMEGA_WIFI_CONTROLLER_ERROR=1)
# add_compile_definitions(CONFIG_OMEGA_FILE_SYSTEM_CONTROLLER_DEBUG=1)
# add_compile_definitions(CONFIG_OMEGA_FILE_SYSTEM_CONTROLLER_INFO=1)
# add_compile_definitions(CONFIG_OMEGA_FILE_SYSTEM_CONTROLLER_WARN=1)
# add_compile_definitions(CONFIG_OMEGA_FILE_SYSTEM_CONTROLLER_ERROR=1)
# add_compile_definitions(CONFIG_OMEGA_WEB_SERVICES_DEBUG=1)
# add_compile_definitions(CONFIG_OMEGA_WEB_SERVICES_INFO=1)
# add_compile_definitions(CONFIG_OMEGA_WEB_SERVICES_WARN=1)
add_compile_definitions(CONFIG_OMEGA_WEB_SERVICES_ERROR=1)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(GET-request)
