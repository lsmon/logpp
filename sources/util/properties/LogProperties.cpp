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

#include <util/properties/LogProperties.hpp>
#include <util/LogUtil.hpp>
#include <utility>

LogProperties::LogProperties() {
    setProperties();
}

LogProperties::LogProperties(const std::string& propertyFile) {
    PropertiesReader config = PropertiesReader::instance(propertyFile);
    if (config.getProperties().empty())
        setProperties();
    else
        setProperties(config);
//    initLogAppender();
}

LogProperties::LogProperties(const PropertiesReader& config) {
    setProperties(config);
//    initLogAppender();
}

LogProperties::LogProperties(const std::string &logLevel, std::string logPath, std::string logFile,
                             std::string mMaxSz) : _log_path(std::move(logPath)), _log_file(std::move(logFile)), _max_sz(std::move(mMaxSz)) {
    this->_log_level = toLogLevel(logLevel);
//    initLogAppender();
}

void LogProperties::setProperties(const PropertiesReader& config) {
    for (auto &p : config.getProperties()) {
        if (p.first == "level")
            setLogLevel(p.second);
        else if (p.first == "path")
            setLogPath(p.second);
        else if (p.first == "file") {
            setLogFile(p.second);
        } else if (p.first == "maxsz")
            setMaxSz(p.second);
        else if (p.first == "roqty")
            setRolloverLimit(p.second);
    }
}

void LogProperties::setProperties() {
    setLogLevel("verbose");
    setLogPath("./");
    setLogFile("logpp-no-version.log");
    setMaxSz("2MB");
    setRolloverLimit("20");
}

//void LogProperties::initLogAppender() {
//    _log_appender = new LogAppender(Util:: buildFileFullPath(this->getLogPath(), this->getLogFile()),
//                                    this->getMaxSzBytes(), this->getRolloverLimit());
//}

Level LogProperties::toLogLevel(std::string level) {
    if (level.compare("INFO") == 0 || level.compare("info") == 0)
        return log_info;
    else if (level.compare("TRACE") == 0 || level.compare("trace") == 0)
        return log_trace;
    else if (level.compare("ERROR") == 0 || level.compare("error") == 0)
        return log_error;
    else if (level.compare("DEBUG") == 0 || level.compare("debug") == 0)
        return log_debug;
    else if (level.compare( "WARNING") == 0 || level.compare("warning") == 0)
        return log_warning;
    else if (level.compare("SILENT") == 0 || level.compare("silent") == 0)
        return log_silent;
    else if (level.compare( "VERBOSE") == 0 || level.compare("verbose") == 0)
        return log_verbose;
    else
        return log_stealth;
}

Level LogProperties::getLogLevel() const {
    return _log_level;
}

void LogProperties::setLogLevel(const std::string &logLevel) {
    this->_log_level = toLogLevel(logLevel);
}

const std::string &LogProperties::getLogPath() const {
    return _log_path;
}

void LogProperties::setLogPath(const std::string &logPath) {
    this->_log_path = logPath;
}

const std::string &LogProperties::getLogFile() const {
    return _log_file;
}

void LogProperties::setLogFile(const std::string &logFile) {
    this->_log_file = logFile;
}

const std::string &LogProperties::getMaxSz() const {
    return _max_sz;
}

void LogProperties::setMaxSz(const std::string &mMaxSz) {
    _max_sz = mMaxSz;
}

int LogProperties::getRolloverLimit() const {
    return _rollover_limit;
}

void LogProperties::setRolloverLimit(const std::string &mRolloverFileQty) {
    _rollover_limit = atoi(mRolloverFileQty.c_str());
}

//LogAppender *LogProperties::getLogAppender() const {
//    return _log_appender;
//}

LogProperties::~LogProperties() {
//    delete _log_appender;
}

long LogProperties::getMaxSzBytes() {
    if (_max_sz.length() < 2) return 2 * MB;
    char prefix = _max_sz.c_str()[_max_sz.length() - 2];
    std::string str_sz = _max_sz.substr(0, _max_sz.length() - 2);
    long sz = atol(str_sz.c_str());
    switch (prefix) {
        case 'K':
            return sz * KB;
        case 'M':
            return sz * MB;
        case 'G':
            return sz * GB;
        default:
            return sz;
    }
}

