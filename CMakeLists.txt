CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#SET (CMAKE_CXX_STANDARD 17)
#SET(CMAKE_C_COMPILER /usr/bin/gcc)
#SET(CMAKE_CXX_COMPILER /usr/bin/g++)
#SET(CMAKE_CXX_FLAGS "-Wall")
#SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
#SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
#-fno-rtti
#libgit2 gtk+-3.0 glfw3 gl glu sqlite3
PROJECT(CaptainBallard)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(CaptainBallard REQUIRED gtk+-3.0 sqlite3 libgit2 appindicator3-0.1)


# Tell the compiler where to look for headers and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${CaptainBallard_INCLUDE_DIRS})
LINK_DIRECTORIES(${CaptainBallard_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${CaptainBallard_CFLAGS_OTHER})

include_directories(src)
file(GLOB SOURCES core/*.c libs/**/*.c ui/*.c util/*.c)
file(GLOB HEADERS core/*.h libs/**/*.h ui/*.h util/*.h)

ADD_EXECUTABLE(CaptainBallard ${SOURCES} ${HEADERS})

# Link the target against the CaptainBallard.0 libraries
TARGET_LINK_LIBRARIES(CaptainBallard ${CaptainBallard_LIBRARIES} pthread) #

set_target_properties(CaptainBallard PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/release
)
