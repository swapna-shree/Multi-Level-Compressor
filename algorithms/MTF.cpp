#include "MTF.h"
#include <list>
#include <stdexcept>
#include<algorithm>

string mtfEncode(const string &input)
{
    list<unsigned char> symbolTable;
    for (int i = 0; i < 256; ++i)
        symbolTable.push_back(i);

    string encoded;
    encoded.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(input[i]);
        auto it = find(symbolTable.begin(), symbolTable.end(), ch);
        if (it == symbolTable.end())
            throw runtime_error("Character not found in symbol table!");

        int index = distance(symbolTable.begin(), it);
        encoded += static_cast<unsigned char>(index); // pack index as a byte

        // Move to front
        symbolTable.erase(it);
        symbolTable.push_front(ch);
    }
    return encoded;
}

string mtfDecode(const string &encoded)
{
    list<unsigned char> symbolTable;
    for (int i = 0; i < 256; ++i)
        symbolTable.push_back(i);

    string decoded;
    decoded.reserve(encoded.size());

    for (size_t i = 0; i < encoded.size(); ++i) {
        unsigned char idx = static_cast<unsigned char>(encoded[i]);
        if (idx >= symbolTable.size())
            throw runtime_error("Invalid index in encoded data!");

        auto it = next(symbolTable.begin(), idx);
        unsigned char ch = *it;
        decoded += ch;

        // Move to front
        symbolTable.erase(it);
        symbolTable.push_front(ch);
    }
    return decoded;
}
