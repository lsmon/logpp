#[[
MIT License

Copyright (c) 2023 Salomon Lee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]
cmake_minimum_required(VERSION 3.16)
project(logpp)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 9)
set(PROJECT_VERSION_TWEAK 2)
set(PROJECT_VERSION_PATCH 5)

set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_TWEAK}.${PROJECT_VERSION_PATCH})

set(PROPERTY ${CMAKE_BINARY_DIR}/resources/logging.properties)
set(CONFIG ${CMAKE_SOURCE_DIR}/include/logconfig.h)

configure_file(logconfig.h.in ${CONFIG})
configure_file(logging.properties.in ${PROPERTY})

set(INC_UTIL_FIO 
        include/util/fio/LogAppender.hpp
        include/util/fio/PropertiesReader.hpp)

set(INC_UTIL_LOGGING 
        include/util/logging/Level.hpp
        include/util/logging/Log.hpp)

set(INC_UTIL_PROPERTIES 
        include/util/properties/LogProperties.hpp)

set(INC_UTIL
        include/util/Date.hpp
        include/util/LogUtil.hpp)

set(INC ${INC_UTIL}
        ${INC_UTIL_FIO}
        ${INC_UTIL_LOGGING}
        ${INC_UTIL_PROPERTIES}
        include/logconfig.h)

set(SRC sources/util/Date.cpp
        sources/util/LogUtil.cpp
        sources/util/fio/LogAppender.cpp
        sources/util/fio/PropertiesReader.cpp
        sources/util/logging/Log.cpp
        sources/util/properties/LogProperties.cpp)

set(SOURCES ${INC} ${SRC})

set(INCLUDE include)
include_directories(${INCLUDE})

add_library(_${PROJECT_NAME}-${PROJECT_VERSION} ${SOURCES})

find_package(ZLIB REQUIRED)

if (ZLIB_FOUND)
target_link_libraries(_${PROJECT_NAME}-${PROJECT_VERSION} PRIVATE ZLIB::ZLIB stdc++fs)
endif ()
link_libraries(stdc++fs)

target_link_options(_${PROJECT_NAME}-${PROJECT_VERSION} PRIVATE -lstdc++fs)
#include(test/CMakeLists.txt)
include(cpack/CMakeLists.txt)