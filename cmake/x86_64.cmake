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
)
FetchContent_MakeAvailable(OmegaUtilityDriver)

FetchContent_Declare(
  cpr 
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  )
FetchContent_MakeAvailable(cpr)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(CPR_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)  # Optional: disable tests to speed up the build
set(CURL_DISABLE_LIBPSL ON CACHE BOOL "Disable libpsl in curl" FORCE)  # Disable libpsl


# Set the runtime library consistently for all targets
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")  # Static runtime in debug mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")  # Static runtime in release mode


add_library(OmegaWebServices STATIC ${PROJ_SOURCES})
target_include_directories(OmegaWebServices PUBLIC ${PROJ_HEADER_DIRS})
target_link_libraries(OmegaWebServices OmegaUtilityDriver cpr::cpr)
# target_link_libraries(OmegaWebServices PRIVATE cpr::cpr)