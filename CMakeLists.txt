cmake_minimum_required(VERSION 3.8)

set(PROJ_HEADER_DIRS inc)
set(PROJ_SOURCES    src/WebServices.cpp)
set(PROJ_DEPENDENCIES OmegaUtilityDriver)

# if(CONFIG_OMEGA_ENABLE_REQUESTS)
    set( PROJ_SOURCES ${PROJ_SOURCES} src/Request.cpp)
    set( PROJ_DEPENDENCIES ${PROJ_DEPENDENCIES} esp_http_client)
# endif()

# if(CONFIG_OMEGA_ENABLE_WEBSOCKET)
    set( PROJ_SOURCES ${PROJ_SOURCES} src/WebSocket.cpp)
    set( PROJ_DEPENDENCIES ${PROJ_DEPENDENCIES} esp_websocket_client)
# endif()

if(ESP_PLATFORM)
    idf_component_register( SRCS            ${PROJ_SOURCES}
                            INCLUDE_DIRS    ${PROJ_HEADER_DIRS}
                            REQUIRES        ${PROJ_DEPENDENCIES}
    )
else()
project(OmegaWebServices)

endif(ESP_PLATFORM)
