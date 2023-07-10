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
#include <utility>
#endif

void LogAppender::moveLogFile() {
    std::string ofname = Util::buildRollbackFileName(_filename);
    std::fstream infile, outfile;
    infile.open(_filename.c_str(), std::ios::in | std::ios::binary);
    outfile.open(ofname.c_str(), std::ios::out | std::ios::binary);
//    std::copy(std::istreambuf_iterator<char>(&infile), {},std::ostreambuf_iterator<char>(&outfile));
#ifdef __APPLE__
    infile.close();
    outfile.close();

    std::string cmd = "gzip -8 ";
    cmd += ofname;
    std::system(cmd.c_str());
#endif
#ifdef __linux__
    compressLog();
#endif
    std::remove(_filename.c_str());
}

void LogAppender::removeOldCompressions() {
    std::string path = Util::recoverFilePath(_filename);
    std::vector<std::string> listOfGzFiles;
    for (const auto &file : std::filesystem::directory_iterator(path)) {
        std::string filename = Util::getNameOfFile(file.path().string());
        std::string extension = Util::getExtensionOfFile(file.path().string());
        if (extension == ".gz" )
            listOfGzFiles.push_back(file.path().string());
    }
    if (listOfGzFiles.size() > _rollover_limit) {
        std::sort(listOfGzFiles.begin(), listOfGzFiles.end(), std::less<std::string>());
        int limit = listOfGzFiles.size() - _rollover_limit;
        for (int i = 0; i < limit; i++) {
            std::remove(listOfGzFiles[i].c_str());
        }
    }
}

LogAppender::LogAppender(std::string mFilename, long mFsz, int roLimit) : _filename(std::move(mFilename)), _file_size(mFsz), _rollover_limit(roLimit) {
    _out_file_stream.open(_filename, std::ios_base::app);
}

LogAppender::~LogAppender() {
    if(_out_file_stream.is_open())
        _out_file_stream.close();

}

void LogAppender::write(const std::string& v) {
    if (!_out_file_stream.is_open()) {
        _out_file_stream.open(_filename, std::ios_base::app);
    }
    _out_file_stream << v;

    if (_out_file_stream.tellp() >= _file_size) {
        moveLogFile();
        removeOldCompressions();
    }
    _out_file_stream.close();
}

bool LogAppender::compressLog() {
    // Open the input file in binary mode
    std::ifstream inputFile(_filename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file: " << _filename << std::endl;
        return false;
    }

    std::string compressed = _filename;
    compressed += ".gz";
    // Open the output file in binary mode
    std::ofstream outputFile(compressed, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << compressed << std::endl;
        inputFile.close();
        return false;
    }

    // Prepare zlib structures
    constexpr int BufferSize = 8192;
    char buffer[BufferSize];

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    // Initialize zlib for compression
    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
        std::cerr << "Error initializing zlib for compression" << std::endl;
        inputFile.close();
        outputFile.close();
        return false;
    }

    // Compress the input file and write the compressed data to the output file
    do {
        inputFile.read(buffer, BufferSize);
        stream.avail_in = static_cast<uInt>(inputFile.gcount());
        stream.next_in = reinterpret_cast<Bytef*>(buffer);

        int ret;
        do {
            stream.avail_out = BufferSize;
            stream.next_out = reinterpret_cast<Bytef*>(buffer);

            ret = deflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR) {
                std::cerr << "Error in zlib deflate: " << stream.msg << std::endl;
                deflateEnd(&stream);
                inputFile.close();
                outputFile.close();
                return false;
            }

            outputFile.write(buffer, BufferSize - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (!inputFile.eof());

    // Clean up zlib
    deflateEnd(&stream);

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    std::cout << "File compressed successfully: " << _filename << " -> " << compressed << std::endl;
    return true;
}

template<class T>
LogAppender &LogAppender::operator<<(const T &v) {
    if (!_out_file_stream.is_open()) {
        _out_file_stream.open(_filename, std::ios_base::app);
    }
    _out_file_stream << v;
    return *this;
}
