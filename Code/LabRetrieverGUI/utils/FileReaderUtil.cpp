//==================================================================================================
// Name        : FileReaderUtil.cpp
// Author      : Ken Cheng
// Copyright   : This program is free software: you can redistribute it and/or modify it under the
//   terms of the GNU General Public License as published by the Free Software Foundation, either
//   version 3 of the License, or (at your option) any later version.  This program is distributed
//   in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
//   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//   more details see <http://www.gnu.org/licenses/>.
// Description : 
//==================================================================================================

#include "FileReaderUtil.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace LabRetriever {
    const string trim(const std::string& pString, const std::string& pWhitespace = " \t\n") {
        const size_t beginStr = pString.find_first_not_of(pWhitespace);
        if (beginStr == std::string::npos)
        {
            // no content
            return "";
        }

        const size_t endStr = pString.find_last_not_of(pWhitespace);
        const size_t range = endStr - beginStr + 1;

        return pString.substr(beginStr, range);
    }

    Race raceFromString(const string& name) {
        if (name == "AFRICAN_AMERICAN") {
            return AFRICAN_AMERICAN;
        } else if (name == "CAUCASIAN") {
            return CAUCASIAN;
        } else if (name == "HISPANIC") {
            return HISPANIC;
        }
        // TODO: fix default
        return AFRICAN_AMERICAN;
    }

    map<string, unsigned int> getAlleleCountsFromFile(const string& fileName, Race race) {
        map<string, unsigned int> retVal;
        ifstream file;
        file.open(fileName.c_str());

        string line;

        // Chomp the first line to get read of headers.
        getline(file, line);
        while (file.good()) {
            unsigned int val;
            getline(file, line);
            if (line.size() == 0) break;
            vector<string> tokenList = makeTokenList(line);
            istringstream(tokenList[(int) race]) >> val;
            retVal[tokenList[0]] = val;
        }

        file.close();

        return retVal;
    }

    vector< vector<string> > readRawCsv(const string& fileName) {
        ifstream file;
        file.open(fileName.c_str());
        string line;

        vector< vector<string> > retVal;

        while (file.good()) {
            getline(file, line);
            if (line.size() == 0) break;
            line = trim(line);
            vector<string> tokenList = makeTokenList(line);
            retVal.push_back(tokenList);
        }

        file.close();

        return retVal;
    }

    /*
     * Quick hack. Does not handle all csv formats.
     */
    vector<string> makeTokenList(const string& line) {
        vector<string> retVal;
        string readInVal = trim(line);
        while (true) {
            // Trimming is not exactly correct, but there should not be whitespace.
            string nextToken = trim(getToken(readInVal));
            retVal.push_back(nextToken);

            if (readInVal.size() == 0)
                break;
            else {
                while (readInVal[0] == ' ') {
                    readInVal.erase(0, 1);
                }
                readInVal.erase(0, 1);
            }
        }
        return retVal;
    }

    /*
     * Gets the next token (an element) from a csv string and advances the string to the next comma.
     */
    string getToken(string& line) {
        while (line.size() != 0 && line[0] == ' ') {
            line.erase(0, 1);
        }

        if (line.size() == 0) return "";

        string retVal;
        if (line[0] == '"') {
            // Continue until you find an unescaped double quote.
            unsigned int index = 1;
            bool isDone = false;
            for (; index < line.size() - 1; index++) {
                char curChar = line[index];
                char nextChar = line[index + 1];
                switch (curChar) {
                    case '\\':
                        switch (nextChar) {
                            case '"':
                            case '\\':
                                // If it's an escaped character, use the escaped character and
                                // advance the index.
                                retVal += nextChar;
                                index++;
                                break;
                            default:
                                retVal += '\\';
                                break;
                        }
                        break;
                    case '"':
                        switch (nextChar) {
                            case '"':
                                retVal += '"';
                                index++;
                                break;
                            case ',':
                                // If it's not a double quote, then it must be the end.
                                isDone = true;
                                break;
                            default:
                                // Should not be here in a valid csv!
                                assert(false);
                                break;
                        }
                        break;
                    default:
                        retVal += curChar;
                        break;
                }

                if (isDone) {
                    break;
                }
            }

            if (!isDone) {
                // This must be the last token.
                if (index != line.size() - 1 || line[index] != '"') {
                    // If the index points past the end of the string or if the last character is
                    // not a double quote, then it was an invalid parse.
                    assert(false);
                }
            }

            // At this point, index should point to the ending double quote.
            line.erase(0, index + 1);
        } else {
            // If unquoted, then there shouldn't be a comma as an element. Just find the next comma
            // and split there.
            int index = line.find(',');
            if (index == string::npos) {
                index = line.size();
            }
            retVal = line.substr(0, index);
            line.erase(0, index);
        }

        return retVal;
    }
}
