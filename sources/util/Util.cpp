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

#include <util/Util.hpp>
#include <util/logging/Log.hpp>
#include <util/Date.hpp>
#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close
#include <ifaddrs.h>    ///< ipv6

std::string Util::buildFileFullPath(std::string path, const std::string& filename) {
    std::string fileFullPath = path;
    std::string fname = filename;
    if (fileFullPath.back() != '/' && filename.c_str()[0] != '/')
        fileFullPath.push_back('/');
    if (filename.c_str()[0] == '/')
        fname.erase(0, 1);
    fileFullPath += fname;
    return fileFullPath;
}

std::string Util::buildRollbackFileName(const std::string& filename) {
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

std::string Util::trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');

    return str.substr(first, (last - first +1));
}

std::string Util::recoverFilePath(const std::string& filename) {
    std::string path;
    size_t dir_pos = filename.find_last_of('/');
    path = filename.substr(0, dir_pos);
    if (path.empty())
        return "./";
    return path;
}

std::vector<std::string> Util::disassembleFileName(const std::string& filename) {
    std::vector<std::string> disassembled = {getPathToFile(filename), getNameOfFile(filename), getExtensionOfFile(filename)};
    return disassembled;
}

std::string Util::getPathToFile(const std::string& filename) {
    size_t dir_pos = filename.find_last_of('/');
    std::string path = (dir_pos > filename.length() || dir_pos < 0)? "" : filename.substr(0, dir_pos);
    return trim(path);
}

std::string Util::getNameOfFile(const std::string& filename) {
    size_t dir_pos = filename.find_last_of('/');
    std::string path = filename.substr(0, dir_pos);
    std::string name = filename.substr(dir_pos+1, filename.length()-1);
    size_t dot = name.find_last_of('.');
    std::string fname = name.substr(0, dot);
    return trim(fname);
}

std::string Util::getExtensionOfFile(const std::string& filename) {
    size_t dot = filename.find_last_of('.');
    std::string extension = (dot > filename.length() || dot < 0)?"":filename.substr(dot, filename.length()-1);
    return trim(extension);
}

std::vector<std::string> Util::decomposeFileGz(const std::string& filename) {
    size_t dash_pos = filename.find_last_of('-');
    size_t dot_pos = filename.find_first_of('.');
    std::string name = filename.substr(0, dash_pos);
    std::string timestamp = filename.substr(dash_pos+1, dot_pos);
    std::string extension = filename.substr(dot_pos, filename.length()-1);
    std::vector<std::string> result = {name, timestamp, extension};
    return result;
}

IpAddress Util::getLocalIpAddress() {
    return {};
}

std::vector<std::string>
Util::getPierListFrom(const std::vector<std::string>& localAddress, const std::vector<std::string>& pierList) {
    return std::vector<std::string>();
}

IpAddress::IpAddress() {
    computeIpV4LocalAddress();
    computeIpV6LocalAddress();
}

const std::vector<std::string> &IpAddress::getIpV4Address() const {
    return _v4address;
}

void IpAddress::computeIpV4LocalAddress() {
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0) {
        std::cerr << "Socket error" << std::endl;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0) {
        LOG_ERROR << "Error number: " << errno
                  << ". Error message: " << strerror(errno);;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);
    if (err == std::string::npos) {

    }
    char buffer[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p != NULL) {
//        LOG_INFO << "Local IP address is: " << buffer;
        _v4address.push_back(buffer);
    } else {
        LOG_ERROR << "Error number: " << errno
                  << ". Error message: " << strerror(errno);
    }
    close(sock);
}

void IpAddress::computeIpV6LocalAddress() {
    ifaddrs* ifaddr, *ifa = nullptr;
    void* addr_ptr = nullptr;
    char addr_str[INET6_ADDRSTRLEN];

    getifaddrs(&ifaddr);

    // Iterate through the list of interfaces
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr){
            continue;
        }

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET6){
            addr_ptr = &(reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr))->sin6_addr;

            // Convert the address to a string and print it
            inet_ntop(AF_INET6, addr_ptr, addr_str, INET6_ADDRSTRLEN);
#ifdef DEBUG
            LOG_DEBUG << "IPv6 address: " << addr_str;
#endif
            _v6address.push_back(addr_str);
        }
    }

    freeifaddrs(ifaddr);
}

const std::vector<std::string> &IpAddress::getIpV6Address() const {
    return _v6address;
}

std::string IpAddress::toString() {
    return std::string();
}
