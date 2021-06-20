#include "OperationString.h"

#include <string>
#include <iostream>
#include <vector>
#include <set>
//#include <map>
//
using std::string;
//using std::map;

//template<typename T, typename P>
//T remove_if(T beg, T end, P pred)
//{
//    T dest = beg;
//    for (T itr = beg; itr != end; ++itr)
//        if (!pred(*itr))
//            *(dest++) = *itr;
//    return dest;
//}

/* for (char c : text)
{
    string symb(1, c);
    result.append(c);
    //result.replace(text.begin(), text.begin() + 6, "");
}*/

string FormatTypeName(string text)
{
    string result = "";
    std::stringstream streamText;
    streamText.str(text);
    streamText >> result; //class
    streamText >> result; //name
    return result;
}

bool IsMapContains_StrInt(map <string, int> inMap, string key)
{
    bool result = false;;
    map <string, int> ::iterator it;
    it = inMap.find(key);
    if (it != inMap.end())
        result = true;
    return result;
}

string Trim(string str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}

bool StrToBool(std::string const& s) {
    return s != "0";
}

bool StrToBool(string str) {
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

bool IsCompareF(float a, float b)
{
    float step = 0.01;
    float res = fabs(a - b);
    return res < step;
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