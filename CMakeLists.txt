cmake_minimum_required(VERSION 3.8)

set(PROJ_HEADER_DIRS inc)
set(PROJ_SOURCES src/WebServices.cpp)

if(ESP_PLATFORM)
    idf_component_register( SRCS            ${PROJ_SOURCES}
                            INCLUDE_DIRS    ${PROJ_HEADER_DIRS}
                            REQUIRES esp_http_client OmegaUtilityDriver
    )
else()
project(OmegaWebServices)

endif(ESP_PLATFORM)
