//==================================================================================================
// Name        : FileReaderUtil.h
// Author      : Ken Cheng
// Copyright   : This program is free software: you can redistribute it and/or modify it under the
//   terms of the GNU General Public License as published by the Free Software Foundation, either
//   version 3 of the License, or (at your option) any later version.  This program is distributed
//   in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
//   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//   more details see <http://www.gnu.org/licenses/>.
// Description : 
//==================================================================================================

#ifndef FILEREADERUTIL_H_
#define FILEREADERUTIL_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace LabRetriever {
    /*
     * Temporary indexing until frequency table formats are finalized.
     */
    enum Race {
        AFRICAN_AMERICAN = 1,
        CAUCASIAN = 3,
        HISPANIC = 5,
    };

    /*
     * Temporary function until frequency table formats are finalized.
     */
    map<string, unsigned int> getAlleleCountsFromFile(const string& fileName, Race race);

    vector< vector<string> > readRawCsv(const string& fileName);

    vector<string> makeTokenList(const string& line);

    string getToken(string& line);
}

#endif /* FILEREADERUTIL_H_ */
