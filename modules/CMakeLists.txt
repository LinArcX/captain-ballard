add_library(
    libgit
    libgit/git_common.c
    libgit/git_common.h)
target_include_directories(libgit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libgit")

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(LIBGIT2 REQUIRED libgit2)
target_link_libraries(libgit ${LIBGIT2_LIBRARIES})
target_include_directories(libgit PUBLIC ${LIBGIT2_INCLUDE_DIRS})
target_compile_options(libgit PUBLIC ${LIBGIT2_CFLAGS_OTHER})


#target_compile_options(libgit PRIVATE ${LIBGIT2_CFLAGS_OTHER})
#target_link_libraries(libgit PRIVATE ${LIBGIT2_LIBRARIES})

#PKG_CHECK_MODULES(libgit REQUIRED libgit2)
#target_include_directories(libgit PUBLIC ${LIBGIT2_INCLUDE_DIRS})

#target_link_libraries(libgit PUBLIC ${LIBGIT2_LIBRARIES})
#target_include_directories(libgit PUBLIC ${LIBGIT2_INCLUDE_DIRS})
#target_compile_options(libgit PUBLIC ${LIBGIT2_CFLAGS_OTHER})

add_library(
    util
    util/cb_log.c
    util/cb_log.h
    util/cb_file.c
    util/cb_file.h
    util/cb_util.c
    util/cb_util.h
    util/cb_array.c
    util/cb_array.h
    util/cb_daemon.c
    util/cb_daemon.h
    util/cb_vector.h)
target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/util")

add_library(
    sqlite
    sqlite/sqlite_util.c
    sqlite/sqlite_util.h)
target_include_directories(sqlite PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/sqlite")
target_link_libraries(sqlite PUBLIC util)

add_library(
    ui
    ui/window_status.c
    ui/window_status.h
    ui/window_settings.c
    ui/window_settings.h
    ui/window_app_indicator.c
    ui/window_app_indicator.h)
target_include_directories(ui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ui")

target_link_libraries(ui PUBLIC ${GTK3_LIBRARIES})
target_include_directories(ui PUBLIC ${GTK3_INCLUDE_DIRS})
target_compile_options(ui PUBLIC ${GTK3_CFLAGS_OTHER})

target_link_libraries(ui PUBLIC sqlite)
target_link_libraries(ui PUBLIC util)
