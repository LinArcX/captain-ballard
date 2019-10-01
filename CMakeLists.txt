CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(captain-ballard)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)
file(GLOB SOURCES core/*.c libs/**/*.c ui/*.c util/c/*.c)
file(GLOB HEADERS core/*.h libs/**/*.h ui/*.h util/c/*.h)

ADD_EXECUTABLE(captain-ballard ${SOURCES} ${HEADERS})

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(captain-ballard REQUIRED gtk+-3.0 sqlite3 libgit2 appindicator3-0.1)

# Tell the compiler where to look for headers and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${captain-ballard_INCLUDE_DIRS})
LINK_DIRECTORIES(${captain-ballard_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${captain-ballard_CFLAGS_OTHER})

# Link the target against the captain-ballard.0 libraries
TARGET_LINK_LIBRARIES(captain-ballard ${captain-ballard_LIBRARIES} pthread)

add_custom_command(
        TARGET captain-ballard
        PRE_BUILD
        COMMAND glib-compile-resources --target=core/resources.c --sourcedir=util/ --generate-source util/resources.xml
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set_target_properties(captain-ballard PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/release
)

#SET (CMAKE_CXX_STANDARD 17)
#SET(CMAKE_C_COMPILER /usr/bin/gcc)
#SET(CMAKE_CXX_COMPILER /usr/bin/g++)
#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
#SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
#-fno-rtti
#libgit2 gtk+-3.0 glfw3 gl glu sqlite3

#SET(CMAKE_INSTALL_PREFIX /usr)
#install(TARGETS captain-ballard DESTINATION bin)
#install(FILES ./util/images/about_us.png DESTINATION share/applications)
