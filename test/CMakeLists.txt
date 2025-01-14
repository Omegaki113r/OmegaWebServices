set(PROJ_TEST_SOURCES GET.cpp)
set(PROJ_TEST_HEADER_DIRS .)

if(ESP_PLATFORM)
    idf_component_register( SRCS ${PROJ_TEST_SOURCES}
                            INCLUDE_DIRS ${PROJ_TEST_HEADER_DIRS}
                            REQUIRES nvs_flash unity OmegaWebServices OmegaWiFiController)
else()
project(OmegaWebServicesTests)
endif(ESP_PLATFORM)