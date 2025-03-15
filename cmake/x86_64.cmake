set(PROJ_HEADER_DIRS        ${PROJ_ROOT_DIR}/inc)
set(PROJ_SOURCES            ${PROJ_ROOT_DIR}/src/WebServices.cpp  ${PROJ_ROOT_DIR}/src/Header.cpp)
# set(PROJ_DEPENDENCIES       OmegaUtilityDriver)

# if(CONFIG_OMEGA_ENABLE_REQUESTS)
    set( PROJ_SOURCES       ${PROJ_SOURCES}  ${PROJ_ROOT_DIR}/src/x86_64.cpp)
# endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  OmegaUtilityDriver
  GIT_REPOSITORY    https://github.com/Omegaki113r/OmegaUtilityDriver.git
  GIT_TAG           origin/main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(OmegaUtilityDriver)

# FetchContent_Declare(
#   Poco
#   GIT_REPOSITORY    https://github.com/pocoproject/poco.git
#   GIT_TAG           origin/main
#   GIT_SHALLOW TRUE
# )
# FetchContent_MakeAvailable(Poco)


FetchContent_Declare(
  paho_mqtt_c
  GIT_REPOSITORY    https://github.com/eclipse-paho/paho.mqtt.c.git
  GIT_TAG           origin/master
  GIT_SHALLOW TRUE
)
set(PAHO_ENABLE_TESTING OFF CACHE BOOL "PAHO_ENABLE_TESTING" FORCE)
set(PAHO_BUILD_SAMPLES OFF CACHE BOOL "PAHO_BUILD_SAMPLES" FORCE)
set(PAHO_BUILD_SHARED OFF CACHE BOOL "PAHO_BUILD_SHARED" FORCE)
set(PAHO_BUILD_STATIC ON CACHE BOOL "PAHO_BUILD_STATIC" FORCE)
FetchContent_MakeAvailable(paho_mqtt_c)

# FetchContent_Declare(
#     paho_mqtt_cpp
#     GIT_REPOSITORY https://github.com/eclipse-paho/paho.mqtt.cpp.git
#     GIT_TAG origin/master
#     GIT_SHALLOW TRUE
# )
# set(PAHO_WITH_MQTT_C ON CACHE BOOL "Build Paho MQTT C library alongside C++ library" FORCE)
# FetchContent_MakeAvailable(paho_mqtt_cpp)

# set(POCO_LIB_PATH ${PROJ_ROOT_DIR}/extra/libs/poco/lib)
# set(POCO_LIB_HEADER_PATH 
#   ${PROJ_ROOT_DIR}/extra/libs/poco/Foundation/include
#   ${PROJ_ROOT_DIR}/extra/libs/poco/Net/include
#   ${PROJ_ROOT_DIR}/extra/libs/poco/Encodings/include
# )

add_library(OmegaWebServices STATIC ${PROJ_SOURCES})
# include_directories(${POCO_LIB_HEADER_PATH})
target_include_directories(OmegaWebServices PUBLIC ${PROJ_HEADER_DIRS} 
  # ${POCO_LIB_HEADER_PATH}
)
target_link_libraries(OmegaWebServices 
  OmegaUtilityDriver 

  paho-mqtt3a-static
  paho-mqtt3c-static
  # ${POCO_LIB_PATH}/PocoNet.lib 
  # ${POCO_LIB_PATH}/PocoNetd.lib 
  # ${POCO_LIB_PATH}/PocoFoundation.lib
  # ${POCO_LIB_PATH}/PocoFoundationd.lib  
  # ${POCO_LIB_PATH}/PocoUtil.lib
  # ${POCO_LIB_PATH}/PocoUtild.lib  
  # ${POCO_LIB_PATH}/PocoEncodings.lib
  # ${POCO_LIB_PATH}/PocoEncodingsd.lib  
)
# target_compile_definitions(OmegaWebServices PUBLIC POCO_OS_FAMILY_WINDOWS)