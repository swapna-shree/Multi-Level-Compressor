#include "RLE.h"
#include <stdexcept>
#include <sstream>

string rleEncoded(const string &input)
{
    if (input.empty())
        return "";

    string result;
    unsigned char currentChar = static_cast<unsigned char>(input[0]);
    unsigned char count = 1;

    for (size_t i = 1; i < input.size(); ++i)
    {
        unsigned char ch = static_cast<unsigned char>(input[i]);
        if (ch == currentChar && count < 255)
        {
            count++;
        }
        else
        {
            result += static_cast<char>(count); 
            result += static_cast<char>(currentChar);             
            currentChar = ch;
            count = 1;
        }
    }
    
    result += static_cast<char>(count);
    result += static_cast<char>(currentChar);

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
        unsigned char ch = static_cast<unsigned char>(input[i + 1]);
        result.append(count, ch);
    }

    return result;
}
