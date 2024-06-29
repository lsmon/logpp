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

#ifndef NLBC_LOGAPPENDER_HPP
#define NLBC_LOGAPPENDER_HPP


#include <string>
#include <fstream>
#include <thread>
#include <logconfig.h>
#include <mutex>

class LogAppender {
private:
    std::string _filename;
    std::ofstream _out_file_stream;
    long _file_size;
    int _rollover_limit;
    std::string _path;
    std::mutex compression_mutex;
    void moveLogFile();

// #ifdef __linux__
    void compressLog(const std::string& inFile);
// #endif
    void removeOldCompressions();

public:

    LogAppender(std::string mFilename,long mFsz, int roLimit, std::string path);

    virtual ~LogAppender();

    void write(const std::string& v);

    template<class T> LogAppender &operator<<(const T &v);
};


#endif //NLBC_LOGAPPENDER_HPP
