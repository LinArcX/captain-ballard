file(GLOB SOURCES *.c)
add_executable(${NAME} ${SOURCES})

FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(LIBGIT2 REQUIRED libgit2)
target_link_libraries(${NAME} ${LIBGIT2_LIBRARIES})
target_include_directories(${NAME} PUBLIC ${LIBGIT2_INCLUDE_DIRS})
target_compile_options(${NAME} PUBLIC ${LIBGIT2_CFLAGS_OTHER})

PKG_CHECK_MODULES(SQLITE3 REQUIRED sqlite3)
target_link_libraries(${NAME} ${SQLITE3_LIBRARIES})
target_include_directories(${NAME} PUBLIC ${SQLITE3_INCLUDE_DIRS})
target_compile_options(${NAME} PUBLIC ${SQLITE3_CFLAGS_OTHER})

PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
target_link_libraries(${NAME} ${GTK3_LIBRARIES})
target_include_directories(${NAME} PUBLIC ${GTK3_INCLUDE_DIRS})
target_compile_options(${NAME} PUBLIC ${GTK3_CFLAGS_OTHER})

PKG_CHECK_MODULES(APPINDICATOR REQUIRED appindicator3-0.1)
target_link_libraries(${NAME} ${APPINDICATOR_LIBRARIES})
target_include_directories(${NAME} PUBLIC ${APPINDICATOR_INCLUDE_DIRS})
target_compile_options(${NAME} PUBLIC ${APPINDICATOR_CFLAGS_OTHER})

# Tell the compiler where to look for headers and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${captain-ballard_INCLUDE_DIRS})
LINK_DIRECTORIES(${captain-ballard_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${captain-ballard_CFLAGS_OTHER})

# Link the target against the captain-ballard.0 libraries
TARGET_LINK_LIBRARIES(captain-ballard ${captain-ballard_LIBRARIES} pthread)

target_link_libraries(${NAME} util)
target_link_libraries(${NAME} ui)
target_link_libraries(${NAME} giti)
target_link_libraries(${NAME} sqlite)

add_custom_command(
    TARGET ${NAME}
    PRE_BUILD
    COMMAND glib-compile-resources --target=resources.c --sourcedir="${CMAKE_CURRENT_SOURCE_DIR}" --generate-source resources.xml
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
