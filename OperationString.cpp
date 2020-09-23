#include "OperationString.h"

#include <string>
#include <iostream>
#include <vector>
#include <set>

using std::string;

std::string GetFile(const char* path)
{
	string pathStr = path;
	//std::size_t found = pathStr.find_last_of("/\\");
	//string file = pathStr.substr(found + 1);
    
    //--------------
    const size_t last_slash_idx = pathStr.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        pathStr.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    const size_t period_idx = pathStr.rfind('.');
    if (std::string::npos != period_idx)
    {
        pathStr.erase(period_idx);
    }
    return pathStr;
	//--------------
    /*
    std::vector<std::string> result;
    std::set<char> delimiters{ '\\' };
    
    char const* start = path;
    for (; *path; ++path)
    {
        if (delimiters.find(*path) != delimiters.end())
        {
            if (start != path)
            {
                std::string str(start, path);
                result.push_back(str);
            }
            else
            {
                result.push_back("");
            }
            start = path + 1;
        }
    }
    result.push_back(start);

    return result.back();
    */
    //-----------------------------
}