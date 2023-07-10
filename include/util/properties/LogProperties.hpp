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
    Level _log_level;
    std::string _log_path;
    std::string _log_file;
    std::string _max_sz;
    int _rollover_limit{};
//    LogAppender *_log_appender;

    Level toLogLevel(std::string level);

    void setProperties(const PropertiesReader& config);

    void setProperties();

//    void initLogAppender();

public:
    LogProperties();

    explicit LogProperties(const std::string& propertyFile);

    explicit LogProperties(const PropertiesReader& config);

    LogProperties(const std::string &logLevel, std::string logPath, std::string logFile,
                  std::string mMaxSz);

    virtual ~LogProperties();

    [[nodiscard]] Level getLogLevel() const;

    void setLogLevel(const std::string &logLevel);

    [[nodiscard]] const std::string &getLogPath() const;

    void setLogPath(const std::string &logPath);

    [[nodiscard]] const std::string &getLogFile() const;

    void setLogFile(const std::string &logFile);

    [[nodiscard]] const std::string &getMaxSz() const;

    void setMaxSz(const std::string &mMaxSz);

    [[nodiscard]] int getRolloverLimit() const;

    void setRolloverLimit(const std::string &mRolloverFileQty);

    long getMaxSzBytes();

//    [[nodiscard]] LogAppender *getLogAppender() const;


};


#endif //UTIL_LOGPROPERTIES_HPP
