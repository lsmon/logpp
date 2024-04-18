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
    _log_properties = nullptr;
    std::thread::id tid = std::this_thread::get_id();
    std::stringstream ss_tid;
    ss_tid << tid;
    _log_level = l;
    _stream << Log::toString(l, true) << " (thx-id: " << ss_tid.str() << ") - " << NOW_TS;
#ifdef DEBUG
    _stream << "- [" << fileName << " - " << funcName << "]: ";
#endif
    std::stringstream ss_ts;
    ss_ts << Date::timestamp();
    _log_line = Log::toString(l, false) + " (thx-id: " + ss_tid.str() + ") - " + NOW_TS + "(" + ss_ts.str() + ") - ";
#ifdef DEBUG
    _log_line += "[" + fileName + " - " + funcName + "]: ";
#endif
}

Log::~Log() {
//    bool LOG_STEALTH = false;
    _stream << "\n";
    std::string line;

    if (_log_properties == nullptr) {
        _log_properties = new LogProperties("./resources/logging.properties");
        defineLogLevels(_log_properties->getLogLevel());
    }
    switch (_log_level) {
        case log_error:
            if (_is_level_error)
                line = _stream.str();
            break;
        case log_debug:
            if (_is_level_debug)
                line = _stream.str();
            break;
        case log_trace:
            if (_is_level_trace)
                line = _stream.str();
            break;
        case log_info:
            if (_is_level_info)
                line = _stream.str();
            break;
        case log_warning:
            if (_is_level_warning)
                line = _stream.str();
            break;
        default:
            line = _stream.str();
            break;
    }

    if (!_is_level_silent) {
        if (!line.empty() && !_is_Level_stealth && !_is_level_silent) {
            std::cout << line;
            LogAppender(_log_properties->getLogFile(),_log_properties->getMaxSzBytes(), _log_properties->getRolloverLimit(), _log_properties->getLogPath())
            .write( _log_line + "\n");
        }
    }
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
            _is_level_debug = true;
            _is_level_error = true;
            _is_level_info = true;
            _is_level_trace = true;
            _is_level_warning = true;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
        case log_stealth:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = false;
            _is_Level_stealth = true;
            _is_level_silent = false;
            break;
        case log_silent:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = false;
            _is_Level_stealth = false;
            _is_level_silent = true;
            break;
        case log_info:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = true;
            _is_level_trace = false;
            _is_Level_stealth = false;
            _is_level_warning = false;
            _is_level_silent = false;
            break;
        case log_debug:
            _is_level_debug = true;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = false;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
        case log_trace:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = true;
            _is_level_warning = false;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
        case log_error:
            _is_level_debug = false;
            _is_level_error = true;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = false;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
        case log_warning:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = true;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
        default:
            _is_level_debug = false;
            _is_level_error = false;
            _is_level_info = false;
            _is_level_trace = false;
            _is_level_warning = false;
            _is_Level_stealth = false;
            _is_level_silent = false;
            break;
    }
}


