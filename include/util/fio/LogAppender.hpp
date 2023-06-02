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
#include <config.h>

class LogAppender {
private:
    std::string m_filename;
    std::ofstream m_ofs;
    long m_fsz;
    int m_rollover_limit;

    void moveLogFile();

    void removeOldCompressions();

public:
    LogAppender(const std::string &mFilename,long mFsz, int roLimit);

    virtual ~LogAppender();

    void write(std::string v);

    template<class T>
    LogAppender &operator<<(const T &v);
};


#endif //NLBC_LOGAPPENDER_HPP
