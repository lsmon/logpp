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
#include <utility>
#include <vector>
#include <iostream>
#include <list>
#include <algorithm>
#ifndef __APPLE__
#include <bits/stdc++.h>
#endif
#include <filesystem>

class LogUtil {
    bool operator()(const std::vector<std::string> & list, std::string a, std::string b) {
        std::vector<std::string> aDecomposeFileGz = decomposeFileGz(std::move(a));
        std::vector<std::string> bDecomposeFileGz = decomposeFileGz(std::move(b));

        // Find the indices of a and b in the list
        auto aIt = std::find(list.begin(), list.end(), a);
        auto bIt = std::find(list.begin(), list.end(), b);

        // Check if both a and b are found in the list
        if (aIt != list.end() && bIt != list.end()) {
            // Calculate the indices of a and b in the list
            size_t aIndex = std::distance(list.begin(), aIt);
            size_t bIndex = std::distance(list.begin(), bIt);

            // Compare the indices
            return aIndex < bIndex;
        } else {
            // If either a or b is not found in the list, consider them equal
            return false;
        }
    }

private:
    static std::vector<std::string> decomposeFileGz(const std::string& filename);

public:
    static std::string buildFileFullPath(std::string path, const std::string& filename);

    static std::string buildRollbackFileName(const std::string& filename);

    static std::string trim(const std::string &str);

    static std::string recoverFilePath(const std::string& filename);

    static std::vector<std::string> disassembleFileName(const std::string& filename);

    static std::string getPathToFile(const std::string& filename);

    static std::string getNameOfFile(const std::string& filename);

    static std::string getExtensionOfFile(const std::string& filename);

    static std::string getFileParentFolder(const std::string& filename);

    static std::string getFilename(const std::string& filename);
};


#endif //UTIL_UTIL_HPP
