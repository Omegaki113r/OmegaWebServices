set(PROJ_HEADER_DIRS        ${PROJ_ROOT_DIR}/inc)
set(PROJ_SOURCES            ${PROJ_ROOT_DIR}/src/WebServices.cpp  ${PROJ_ROOT_DIR}/src/Header.cpp)
set(PROJ_DEPENDENCIES       OmegaUtilityDriver)

# if(CONFIG_OMEGA_ENABLE_REQUESTS)
    set( PROJ_SOURCES       ${PROJ_SOURCES}  ${PROJ_ROOT_DIR}/src/ESP32xx.cpp)
    set( PROJ_DEPENDENCIES  ${PROJ_DEPENDENCIES} esp_http_client)
# endif()

# if(CONFIG_OMEGA_ENABLE_WEBSOCKET)
    set( PROJ_SOURCES       ${PROJ_SOURCES} ${PROJ_ROOT_DIR}/src/WebSocket.cpp)
    set( PROJ_DEPENDENCIES  ${PROJ_DEPENDENCIES} esp_websocket_client)
# endif()

# if(CONFIG_OMEGA_ENABLE_MQTT)
    # set( PROJ_SOURCES       ${PROJ_SOURCES} ${PROJ_ROOT_DIR}/src/MQTT.cpp)
    set( PROJ_DEPENDENCIES  ${PROJ_DEPENDENCIES} mqtt)
# endif()

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    ${PROJ_HEADER_DIRS}
                        REQUIRES        ${PROJ_DEPENDENCIES}
)