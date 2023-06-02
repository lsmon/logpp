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
#include <util/Util.hpp>

LogProperties::LogProperties() {
    setProperties();
}

LogProperties::LogProperties(std::string propertyFile) {
    PropertiesReader config = PropertiesReader::instance(propertyFile);
    if (config.getProperties().empty())
        setProperties();
    else
        setProperties(config);
    initLogAppender();
}

LogProperties::LogProperties(PropertiesReader config) {
    setProperties(config);
    initLogAppender();
}

LogProperties::LogProperties(const std::string &logLevel, const std::string &logPath, const std::string &logFile,
                             const std::string &mMaxSz) : logPath(logPath), logFile(logFile), maxSz(mMaxSz) {
    this->logLevel = toLogLevel(logLevel);
    initLogAppender();
}

void LogProperties::setProperties(PropertiesReader config) {
    for (auto &p : config.getProperties()) {
        if (p.first.compare("level") == 0)
            setLogLevel(p.second);
        else if (p.first.compare("path") == 0)
            setLogPath(p.second);
        else if (p.first.compare("file") == 0)
            setLogFile(p.second);
        else if (p.first.compare("maxsz") == 0)
            setMaxSz(p.second);
        else if (p.first.compare("roqty") == 0)
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

void LogProperties::initLogAppender() {
    logAppender = new LogAppender(Util:: buildFileFullPath(this->getLogPath(), this->getLogFile()),
                                  this->getMaxSzBytes(), this->getRolloverLimit());
}

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
    return logLevel;
}

void LogProperties::setLogLevel(const std::string &logLevel) {
    this->logLevel = toLogLevel(logLevel);
}

const std::string &LogProperties::getLogPath() const {
    return logPath;
}

void LogProperties::setLogPath(const std::string &logPath) {
    this->logPath = logPath;
}

const std::string &LogProperties::getLogFile() const {
    return logFile;
}

void LogProperties::setLogFile(const std::string &logFile) {
    this->logFile = logFile;
}

const std::string &LogProperties::getMaxSz() const {
    return maxSz;
}

void LogProperties::setMaxSz(const std::string &mMaxSz) {
    maxSz = mMaxSz;
}

const int LogProperties::getRolloverLimit() const {
    return rolloverLimit;
}

void LogProperties::setRolloverLimit(const std::string &mRolloverFileQty) {
    rolloverLimit = atoi(mRolloverFileQty.c_str());
}

LogAppender *LogProperties::getLogAppender() const {
    return logAppender;
}

LogProperties::~LogProperties() {
    delete logAppender;
}

long LogProperties::getMaxSzBytes() {
    if (maxSz.length() < 2) return 2 * MB;
    char prefix = maxSz.c_str()[maxSz.length() - 2];
    std::string str_sz = maxSz.substr(0, maxSz.length() - 2);
    int sz = atol(str_sz.c_str());
    switch (prefix) {
        case 'K':
            return sz * KB;
        case 'M':
            return sz * MB;
        case 'G':
            return sz * GB;
        default:
            return atol(maxSz.c_str());
    }
}

