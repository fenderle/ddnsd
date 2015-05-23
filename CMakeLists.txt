cmake_minimum_required (VERSION 2.6)

project (ddnsd)
set (ddnsd_VERSION_MAJOR 2)
set (ddnsd_VERSION_MINOR 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

add_executable (ddnsd src/main.cpp src/config.cpp src/app.cpp)

find_package (CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(ddnsd ${CURL_LIBRARIES})
endif ()

find_package (PCRE REQUIRED)
if (PCRE_FOUND)
    include_directories(${PCRE_INCLUDE_DIRS})
    target_link_libraries(ddnsd ${PCRE_LIBRARIES})
endif ()

find_package (LDNS REQUIRED)
if (LDNS_FOUND)
    include_directories(${LDNS_INCLUDE_DIRS})
    target_link_libraries(ddnsd ${LDNS_LIBRARIES})
endif ()
