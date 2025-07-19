#include "RLE.h"
#include <stdexcept>
#include <sstream>

string rleEncoded(const string &input)
{
    if (input.empty())
        return "";

    string result;
    char currentChar = input[0];
    unsigned char count = 1;

    for (size_t i = 1; i < input.size(); ++i)
    {
        if (input[i] == currentChar && count < 255)
        {
            count++;
        }
        else
        {
            result += static_cast<char>(count); 
            result += currentChar;             
            currentChar = input[i];
            count = 1;
        }
    }
    
    result += static_cast<char>(count);
    result += currentChar;

    return result;
}

string rleDecoded(const string &input)
{
    if (input.size() % 2 != 0)
    {
        throw runtime_error("Invalid RLE data: length must be even.");
    }

    string result;

    for (size_t i = 0; i < input.size(); i += 2)
    {
        unsigned char count = static_cast<unsigned char>(input[i]);
        char ch = input[i + 1];
        result.append(count, ch);
    }

    return result;
}
