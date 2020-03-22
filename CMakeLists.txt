cmake_minimum_required(VERSION 3.12)

SET(NAME captain-ballard)
project(${NAME} LANGUAGES C)
project(${NAME} VERSION 1.0.0)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)

add_subdirectory(core)
add_subdirectory(modules)

set_target_properties(${NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/release
)
