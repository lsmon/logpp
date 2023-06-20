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
cmake_minimum_required(VERSION 3.24)
project(logpp)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 9)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_TWEAK}.${PROJECT_VERSION_PATCH})

set(PROPERTY ${CMAKE_BINARY_DIR}/resources/logging.properties)
set(CONFIG ${CMAKE_SOURCE_DIR}/include/config.h)

configure_file(config.h.in ${CONFIG})
configure_file(logging.properties.in ${PROPERTY})

set(INC include/util/Date.hpp
        include/util/Util.hpp
        include/util/fio/LogAppender.hpp
        include/util/fio/PropertiesReader.hpp
        include/util/logging/Level.hpp
        include/util/logging/Log.hpp
        include/util/properties/LogProperties.hpp)

set(SRC sources/util/Date.cpp
        sources/util/Util.cpp
        sources/util/fio/LogAppender.cpp
        sources/util/fio/PropertiesReader.cpp
        sources/util/logging/Log.cpp
        sources/util/properties/LogProperties.cpp)

set(SOURCES ${INC} ${SRC})

set(INCLUDE include)
include_directories(${INCLUDE})

add_library(_${PROJECT_NAME} ${SOURCES})

include(cmake/test.cmake)
