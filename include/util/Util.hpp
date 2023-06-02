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

#ifndef UTIL_UTIL_HPP
#define UTIL_UTIL_HPP

#include <string>
#include <vector>

class IpAddress;

class IpAddress {
private:
    std::vector<std::string> v4address;
    std::vector<std::string> v6address;
    void computeIpV4LocalAddress();
    void computeIpV6LocalAddress();

public:
    IpAddress();

    const std::vector<std::string> &getIpV4Address() const;

    const std::vector<std::string> &getIpV6Address() const;

    std::string toString();
};

class Util {
    struct CompareStringFunctions {
        bool operator()(const std::vector<std::string> & list, std::string a, std::string b) {
            std::vector<std::string> aDecomposeFileGz = decomposeFileGz(a);
            std::vector<std::string> bDecomposeFileGz = decomposeFileGz(b);

            long ats = std::stol(aDecomposeFileGz[1]);
            long bts = std::stol(bDecomposeFileGz[1]);

            return ats < bts;
        }
    };

private:
    static std::vector<std::string> decomposeFileGz(std::string filename);

public:
    static std::string buildFileFullPath(std::string path, std::string filename);

    static std::string buildRollbackFileName(std::string filename);

    static std::string trim(const std::string &str);

    static std::string recoverFilePath(std::string filename);

    static std::vector<std::string> disassembleFileName(std::string filename);

    static std::string getPathToFile(std::string filename);

    static std::string getNameOfFile(std::string filename);

    static std::string getExtensionOfFile(std::string filename);

    static IpAddress getLocalIpAddress();

    static std::vector<std::string> getPierListFrom(std::vector<std::string> localAddress, std::vector<std::string> pierList);

};


#endif //UTIL_UTIL_HPP
