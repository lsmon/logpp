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

#include <util/fio/LogAppender.hpp>
#include <iostream>
#include <util/Util.hpp>

#ifdef __linux__
#include <zlib.h>
#include <filesystem>
#include <algorithm>
#endif

void LogAppender::moveLogFile() {
    std::string ofname = Util::buildRollbackFileName(m_filename);
    std::filebuf infile, outfile;
    infile.open(m_filename.c_str(), std::ios::in | std::ios::binary);
    outfile.open(ofname.c_str(), std::ios::out | std::ios::binary);
    std::copy(std::istreambuf_iterator<char>(&infile), {},
              std::ostreambuf_iterator<char>(&outfile));
#ifdef APPLE
    infile.close();
    outfile.close();

    std::string cmd = "gzip -8 ";
    cmd += ofname;
    std::system(cmd.c_str());
#endif
#ifdef UNIX
    std::string cofname = ofname;
    cofname += ".gz";
    infile.open(ofname.c_str(), std::ios::in | std::ios::binary);
    gzFile fi = (gzopen(cofname.c_str(), "wb"));
    gzwrite(fi, infile.rdbuf(), infile.tellp());
    gzclose(fi);

#endif
    std::remove(m_filename.c_str());
}

void LogAppender::removeOldCompressions() {
    std::string path = Util::recoverFilePath(m_filename);
    std::vector<std::string> listOfGzFiles;
    for (const auto &file : std::filesystem::directory_iterator(path)) {
        std::string filename = Util::getNameOfFile(file.path().string());
        std::string extension = Util::getExtensionOfFile(file.path().string());
        if (extension.compare(".gz") == 0 )
            listOfGzFiles.push_back(file.path().string());
    }
    if (listOfGzFiles.size() > m_rollover_limit) {
        std::sort(listOfGzFiles.begin(), listOfGzFiles.end(), std::less<std::string>());
        int limit = listOfGzFiles.size() - m_rollover_limit;
        for (int i = 0; i < limit; i++) {
//            std::cout << "Removing: " << listOfGzFiles[i] << std::endl;
            std::remove(listOfGzFiles[i].c_str());
        }
    }
}

LogAppender::LogAppender(const std::string &mFilename, long mFsz, int roLimit) : m_filename(mFilename), m_fsz(mFsz), m_rollover_limit(roLimit) {
    m_ofs.open(m_filename, std::ios_base::app | std::ios_base::binary | std::ios_base::ate);
}

LogAppender::~LogAppender() {
    m_ofs.close();
}

void LogAppender::write(std::string v) {
    if (!m_ofs.is_open()) {
        m_ofs.open(m_filename, std::ios_base::app);
    }
    m_ofs << v;
    if (m_ofs.tellp() >= m_fsz) {
        m_ofs.close();
        moveLogFile();
        removeOldCompressions();
    }
}

template<class T>
LogAppender &LogAppender::operator<<(const T &v) {
    if (!m_ofs.is_open()) {
        m_ofs.open(m_filename, std::ios_base::app);
    }
    m_ofs << v;
}
