#include "OperationString.h"

#include <string>
#include <iostream>
#include <vector>
#include <set>

using std::string;

//template<typename T, typename P>
//T remove_if(T beg, T end, P pred)
//{
//    T dest = beg;
//    for (T itr = beg; itr != end; ++itr)
//        if (!pred(*itr))
//            *(dest++) = *itr;
//    return dest;
//}

string Trim(string str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}

bool Str_bool(std::string const& s) {

    //bool b;
    //std::istringstream m("1") >> b;

    return s != "0";
}

std::string GetFile(const char* path)
{
	string pathStr = path;
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