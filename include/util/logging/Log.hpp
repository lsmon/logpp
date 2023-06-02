// MIT License
//
// Copyright (c) 2023 Salomon Lee
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef UTIL_LOG_HPP
#define UTIL_LOG_HPP

#include <sstream>
#include <util/Date.hpp>
#include <util/logging/Level.hpp>
#include <util/properties/LogProperties.hpp>
#include <config.h>

class Log {
private:
    std::stringstream stream;
    Level logLevel;
    LogProperties *logProperties;
    std::string logLine;
    bool isLevelDebug;
    bool isLevelTrace;
    bool isLevelError;
    bool isLevelInfo;
    bool isLevelWarning;
    bool isLevelStealth;
    bool isLevelSilent;

    static std::string toString(Level l, bool isStdOut);

    void defineLogLevels(Level level);

public:
    Log(const std::string &fileName, const std::string &funcName, Level l);

    virtual ~Log();

    template<class T>
    Log &operator<<(const T &v) {
        std::stringstream ss;
        ss << v;
        stream << ss.str();
        logLine += ss.str();
        return *this;
    }
};

#ifdef __APPLE__
#define LOG_INFO  Log(__FILE_NAME__, __PRETTY_FUNCTION__, log_info)
#define LOG_WARN  Log(__FILE_NAME__, __PRETTY_FUNCTION__, log_warning)
#define LOG_TRACE Log(__FILE_NAME__, __PRETTY_FUNCTION__, log_trace)
#define LOG_ERROR Log(__FILE_NAME__, __PRETTY_FUNCTION__, log_error)
#define LOG_DEBUG Log(__FILE_NAME__, __PRETTY_FUNCTION__, log_debug)
#define LOG Log(__FILE_NAME__, __FUNCTION__, log_verbose)
#endif
#ifdef __linux__
#define LOG_INFO  Log(__FILE__, __PRETTY_FUNCTION__, log_info)
#define LOG_WARN  Log(__FILE__, __PRETTY_FUNCTION__, log_warning)
#define LOG_TRACE Log(__FILE__, __PRETTY_FUNCTION__, log_trace)
#define LOG_ERROR Log(__FILE__, __PRETTY_FUNCTION__, log_error)
#define LOG_DEBUG Log(__FILE__, __PRETTY_FUNCTION__, log_debug)
#define LOG Log(__FILE__, __FUNCTION__, log_verbose)
#endif

#define NOW_TS Date::nowTime()

#endif //UTIL_LOG_HPP
