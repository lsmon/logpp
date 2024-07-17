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

#include <util/LogUtil.hpp>
#include <util/logging/Log.hpp>
#include <util/Date.hpp>
#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <cerrno>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close
#include <ifaddrs.h>    ///< ipv6
#include <filesystem>

std::string LogUtil::buildFileFullPath(std::string path, const std::string& filename) {
    std::string fileFullPath = path;
    std::string fname = filename;
    if (fileFullPath.back() != '/')
        fileFullPath.push_back('/');
    if (filename.c_str()[0] == '/' )
        fname = fname.substr(1);
    fileFullPath += fname;
    return fileFullPath;
}

std::string LogUtil::buildRollbackFileName(const std::string& filename) {
    size_t dot = filename.find_last_of('.');
    std::string name = filename.substr(0, dot);
    std::string extension = getExtensionOfFile(filename);
    name += "-" + std::to_string(Date::timestamp());
    name += extension;
    size_t pos;
    while ((pos = name.find(" ")) != std::string::npos) {
        name.replace(pos, 1, "_");
    }
    return name;
}

std::string LogUtil::trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');

    return str.substr(first, (last - first +1));
}

std::string LogUtil::recoverFilePath(const std::string& filename) {
    size_t dir_pos = filename.find_last_of('/');
    if (dir_pos == std::string::npos) return "./";
    std::string path = filename.substr(0, dir_pos);
    if (path.empty())
        return "./";
    return path;
}

std::vector<std::string> LogUtil::disassembleFileName(const std::string& filename) {
    std::vector<std::string> disassembled = {getPathToFile(filename), getNameOfFile(filename), getExtensionOfFile(filename)};
    return disassembled;
}

std::string LogUtil::getPathToFile(const std::string& filename) {
    size_t dir_pos = filename.find_last_of('/');
    std::string path = (dir_pos > filename.length())? "" : filename.substr(0, dir_pos);
    trim(path);
    return path;
}

std::string LogUtil::getNameOfFile(const std::string& filename) {
    size_t dir_pos = filename.find_last_of('/');
    std::string path = filename.substr(0, dir_pos);
    std::string name = filename.substr(dir_pos+1, filename.length()-1);
    size_t dot = name.find_last_of('.');
    std::string fname = name.substr(0, dot);
    trim(fname);
    return fname;
}

std::string LogUtil::getExtensionOfFile(const std::string& filename) {
    size_t dot = filename.find_last_of('.');
    std::string extension = (dot > filename.length())?"":filename.substr(dot, filename.length()-1);
    trim(extension);
    return extension;
}

std::vector<std::string> LogUtil::decomposeFileGz(const std::string& filename) {
    size_t dash_pos = filename.find_last_of('-');
    size_t dot_pos = filename.find_first_of('.');
    std::string name = filename.substr(0, dash_pos);
    std::string timestamp = filename.substr(dash_pos+1, dot_pos);
    std::string extension = filename.substr(dot_pos, filename.length()-1);
    std::vector<std::string> result = {name, timestamp, extension};
    return result;
}

std::string LogUtil::getFileParentFolder(const std::string &filename) {
    if (std::filesystem::is_regular_file(filename))
        return std::filesystem::path(filename).parent_path().string();
    else if (std::filesystem::is_directory(filename))
        return filename;
    return "";
}

std::string LogUtil::getFilename(const std::string &filename) {
    size_t dir_pos = filename.find_last_of('/');
    return (dir_pos == std::string::npos)? filename : filename.substr(dir_pos+1, filename.length()-1);
}
