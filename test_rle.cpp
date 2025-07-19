#include "RLE.h"
#include <iostream>
using namespace std;

void test(const string &input, const string &desc)
{
    string encoded = rleEncoded(input);
    string decoded = rleDecoded(encoded);

    cout << desc << endl;
    cout << "Original: " << input << endl;
    cout << "Encoded (hex): ";
    for (unsigned char c : encoded)
        printf("%02X ", c);
    cout << endl;
    cout << "Decoded: " << decoded << endl;

    if (decoded == input)
    {
        cout << "Test passed: decoded string matches original.\n"
             << endl;
    }
    else
    {
        cout << "Test failed: decoded string does not match original.\n"
             << endl;
    }
}

int main()
{
    test("aaabbccccdd", "Testing basic string...");
    test("", "Testing empty string...");
    test("A", "Testing single character...");
    test(string(300, 'x'), "Testing long repeat (300 x)...");
    test("112233445566", "Testing numeric string...");
    test("a1b2c3d4", "Testing mixed symbols...");
    const char rawData[] = {0x00, 0x00, 0x00, 'a', 'b', 'c'};
    test(string(rawData, sizeof(rawData)), "Testing binary data...");

    return 0;
}
