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

#include <util/logging/Log.hpp>
#include <thread>
#include <iostream>


Log::Log(const std::string &fileName, const std::string &funcName, Level l) {
    std::thread::id tid = std::this_thread::get_id();
    std::stringstream ss_tid;
    ss_tid << tid;
    logLevel = l;
    stream << Log::toString(l, true) << " (thx-id: " << ss_tid.str() << ") - " << NOW_TS << "- [" << fileName << " - " << funcName << "]: ";
    std::stringstream ss_ts;
    ss_ts << Date::timestamp();
    logLine = Log::toString(l, false) + " (thx-id: " + ss_tid.str() + ") - " + NOW_TS + "(" + ss_ts.str() + ") - [" + fileName + " - " + funcName + "]: ";
    logProperties = new LogProperties("./resources/logging.properties");
    defineLogLevels(logProperties->getLogLevel());
}

Log::~Log() {
    bool LOG_STEALTH = false;
    stream << "\n";
    std::string line;

    switch (logLevel) {
        case log_error:
            if (isLevelError)
                line = stream.str();
            break;
        case log_debug:
            if (isLevelDebug)
                line = stream.str();
            break;
        case log_trace:
            if (isLevelTrace)
                line = stream.str();
            break;
        case log_info:
            if (isLevelInfo)
                line = stream.str();
            break;
        case log_warning:
            if (isLevelWarning)
                line = stream.str();
            break;
        default:
            line = stream.str();
            break;
    }

    if (!line.empty() && !isLevelStealth && !isLevelSilent)
        std::cout << line;
    if (!isLevelSilent)
        logProperties->getLogAppender()->write(logLine + "\n");
}

std::string Log::toString(Level l, bool isStdOut) {
    std::string level;
    switch (l) {
        case log_info:
            level = (isStdOut)?"\033[32mINFO\033[0m    |":"INFO    |";
            break;
        case log_trace:
            level = (isStdOut)?"\033[33mTRACE\033[0m   |":"TRACE   |";
            break;
        case log_error:
            level = (isStdOut)?"\033[41mERROR\033[0m   |":"ERROR   |";
            break;
        case log_debug:
            level = (isStdOut)?"\033[34mDEBUG\033[0m   |":"DEBUG   |" ;
            break;
        case log_warning:
            level = (isStdOut)?"\033[35mWARNING\033[0m |":"WARNING |" ;
            break;
        default:
            level = (isStdOut)?" \033[42mLOG\033[0m    |":"LOG     |" ;
            break;
    }
    return level;
}

void Log::defineLogLevels(Level level) {
    switch (level) {
        case log_verbose:
            isLevelDebug = true;
            isLevelError = true;
            isLevelInfo = true;
            isLevelTrace = true;
            isLevelWarning = true;
            isLevelStealth = false;
            isLevelSilent = false;
            break;
        case log_stealth:
            isLevelDebug = false;
            isLevelError = false;
            isLevelInfo = false;
            isLevelTrace = false;
            isLevelWarning = false;
            isLevelStealth = true;
            isLevelSilent = false;
            break;
        case log_silent:
            isLevelDebug = false;
            isLevelError = false;
            isLevelInfo = false;
            isLevelTrace = false;
            isLevelWarning = false;
            isLevelStealth = false;
            isLevelSilent = true;
            break;
        case log_info:
            isLevelDebug = false;
            isLevelError = false;
            isLevelInfo = true;
            isLevelTrace = false;
            isLevelStealth = false;
            isLevelWarning = false;
            isLevelSilent = false;
            break;
        case log_debug:
            isLevelDebug = true;
            isLevelError = false;
            isLevelInfo = false;
            isLevelTrace = false;
            isLevelWarning = false;
            isLevelStealth = false;
            isLevelSilent = false;
            break;
        case log_trace:
            isLevelDebug = false;
            isLevelError = false;
            isLevelInfo = false;
            isLevelTrace = true;
            isLevelWarning = false;
            isLevelStealth = false;
            isLevelSilent = false;
            break;
        case log_error:
            isLevelDebug = false;
            isLevelError = true;
            isLevelInfo = false;
            isLevelTrace = false;
            isLevelWarning = false;
            isLevelStealth = false;
            isLevelSilent = false;
            break;
        default:
            isLevelDebug = false;
            isLevelError = false;
            isLevelInfo = false;
            isLevelTrace = false;
            isLevelWarning = false;
            isLevelStealth = false;
            isLevelSilent = false;
            break;
    }
}


