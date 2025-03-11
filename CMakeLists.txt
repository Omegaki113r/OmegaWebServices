cmake_minimum_required(VERSION 3.8)

set(PROJ_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
if(ESP_PLATFORM)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/ESP32xx.cmake)
else()
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/x86_64.cmake) 
endif()
