# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.16)

set( WEB_SERVICES_PATH ../.. )
set( EXTRA_COMPONENT_DIRS ${WEB_SERVICES_PATH} )

set(TEST_COMPONENTS OmegaWebServices)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(unit-test)
