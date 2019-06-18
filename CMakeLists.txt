cmake_minimum_required(VERSION 2.8)
project(ftpdownload)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
else ()
    set (CMAKE_C_STANDARD 99)
endif ()

set(SOURCE_FILES main.c config.h config.c curlftp.h curlftp.c def.h log.c log.h)
add_executable(ftpdownload ${SOURCE_FILES})
target_link_libraries(ftpdownload curl)
