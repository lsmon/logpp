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

#ifndef UTIL_LOG_PROPERTIES_HPP
#define UTIL_LOG_PROPERTIES_HPP

#include <util/logging/Level.hpp>

#include <string>
#include <util/fio/LogAppender.hpp>
#include <util/fio/PropertiesReader.hpp>

class LogProperties {
private:
    Level logLevel;
    std::string logPath;
    std::string logFile;
    std::string maxSz;
    int rolloverLimit;
    LogAppender *logAppender;

    Level toLogLevel(std::string level);

    void setProperties(PropertiesReader config);

    void setProperties();

    void initLogAppender();

public:
    LogProperties();

    LogProperties(std::string propertyFile);

    LogProperties(PropertiesReader config);

    LogProperties(const std::string &logLevel, const std::string &logPath, const std::string &logFile,
                  const std::string &mMaxSz);

    virtual ~LogProperties();

    Level getLogLevel() const;

    void setLogLevel(const std::string &logLevel);

    const std::string &getLogPath() const;

    void setLogPath(const std::string &logPath);

    const std::string &getLogFile() const;

    void setLogFile(const std::string &logFile);

    const std::string &getMaxSz() const;

    void setMaxSz(const std::string &mMaxSz);

    const int getRolloverLimit() const;

    void setRolloverLimit(const std::string &mRolloverFileQty);

    long getMaxSzBytes();

    LogAppender *getLogAppender() const;


};


#endif //UTIL_LOGPROPERTIES_HPP
