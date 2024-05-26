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

#include "util/fio/LogAppender.hpp"
#include <iostream>
#include <util/Util.hpp>

#include <zlib.h>
#include <filesystem>
#include <algorithm>
#include <utility>
#include <mutex>

void LogAppender::moveLogFile()
{
    compression_mutex.lock();
    std::string ofname = Util::buildRollbackFileName(_filename);

    std::ifstream infile(_filename.c_str(), std::ios::binary);
    std::ofstream outfile(ofname, std::ios::binary);
    if (infile.is_open() && outfile.is_open())
    {
        std::copy(std::istreambuf_iterator<char>(infile),
                  std::istreambuf_iterator<char>(),
                  std::ostreambuf_iterator<char>(outfile));
    }

    infile.close();
    outfile.close();
    std::ofstream logFileTotruncate(_filename, std::ios::trunc);
    if (logFileTotruncate.is_open())
    {
        std::cout << _filename << " has been emptied succesfully. " << std::endl;
    }
    else
    {
        std::cerr << _filename << " has failed emptying its content. " << std::endl;
    }
    logFileTotruncate.close();

    compressLog(ofname);
    compression_mutex.unlock();
}

void LogAppender::removeOldCompressions()
{
    compression_mutex.lock();
    std::string path = _path;
    std::vector<std::string> listOfGzFiles;
    for (const auto &file : std::filesystem::directory_iterator(path))
    {
        std::string filename = Util::getNameOfFile(file.path().string());
        std::string extension = Util::getExtensionOfFile(file.path().string());
        if (extension == ".gz")
            listOfGzFiles.push_back(file.path().string());
    }
    if (listOfGzFiles.size() > _rollover_limit)
    {
        std::sort(listOfGzFiles.begin(), listOfGzFiles.end(), std::less<std::string>());
        int limit = listOfGzFiles.size() - _rollover_limit;
        for (int i = 0; i < limit; i++)
        {
            std::remove(listOfGzFiles[i].c_str());
        }
    }
    compression_mutex.unlock();
}

LogAppender::LogAppender(std::string mFilename, long mFsz, int roLimit, std::string path) : _filename(std::move(mFilename)), _file_size(mFsz), _rollover_limit(roLimit), _path(std::move(path))
{
    _out_file_stream.open(_filename, std::ios_base::app);
}

LogAppender::~LogAppender()
{
    if (_out_file_stream.is_open())
        _out_file_stream.close();
}

void LogAppender::write(const std::string &v)
{
    if (!_out_file_stream.is_open())
    {
        _out_file_stream.open(_filename, std::ios_base::app);
    }
    _out_file_stream << v;

    if (_out_file_stream.tellp() >= _file_size)
    {
        std::thread compressionThread([&]()
                                      { moveLogFile(); });
        std::thread removeThread([&]()
                                 { removeOldCompressions(); });
        compressionThread.join();
        removeThread.join();
    }
    _out_file_stream.close();
}

// #ifdef __linux__
void LogAppender::compressLog(const std::string &inFile)
{
    // Open the input file in binary mode
    std::ifstream inputFile(inFile, std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening input file: " << inFile << std::endl;
    }

    std::string compressed = inFile;
    compressed += ".zip";
    // Open the output file in binary mode
    std::ofstream outputFile(compressed, std::ios::binary);
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening output file: " << compressed << std::endl;
        inputFile.close();
    }

    // Prepare zlib structures
    constexpr int BufferSize = 8192;
    char buffer[BufferSize];

    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    // Initialize zlib for compression
    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        std::cerr << "Error initializing zlib for compression" << std::endl;
    }

    // Compress the input file and write the compressed data to the output file
    do
    {
        inputFile.read(buffer, BufferSize);
        stream.avail_in = static_cast<uInt>(inputFile.gcount());
        stream.next_in = reinterpret_cast<Bytef *>(buffer);

        int ret;
        do
        {
            stream.avail_out = BufferSize;
            stream.next_out = reinterpret_cast<Bytef *>(buffer);

            ret = deflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR)
            {
                std::cerr << "Error in zlib deflate: " << stream.msg << std::endl;
                deflateEnd(&stream);
                inputFile.close();
                outputFile.close();
            }

            outputFile.write(buffer, BufferSize - stream.avail_out);
        } while (stream.avail_out == 0);
    } while (!inputFile.eof());

    // Clean up zlib
    deflateEnd(&stream);

    // Close the input and output files
    inputFile.close();
    outputFile.close();
    std::remove(inFile.c_str());
    std::cout << "File compressed successfully: " << inFile << " -> " << compressed << std::endl;
}
// #endif

template <class T>
LogAppender &LogAppender::operator<<(const T &v)
{
    if (!_out_file_stream.is_open())
    {
        _out_file_stream.open(_filename, std::ios_base::app);
    }
    _out_file_stream << v;
    return *this;
}
