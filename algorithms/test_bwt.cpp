#include "BWT.h"
#include <iostream>
#include <string>
#include <cassert>
#include <random>

using namespace std;

void testBWT(const string &input, const string &desc)
{
    cout << "[Test] " << desc << endl;

    int primaryIndex = 0;
    string encoded = bwtEncode(input, primaryIndex);
    string decoded = bwtDecode(encoded, primaryIndex);

    cout << "Original: " << input << endl;
    cout << "Encoded:  " << encoded << endl;
    cout << "Decoded:  " << decoded << endl;
    cout << "Primary Index: " << primaryIndex << endl;

    if (decoded == input)
    {
        cout << "Passed\n\n";
    }
    else
    {
        cout << "Failed\n";
        cout << "Expected: " << input << endl;
        cout << "Got:      " << decoded << endl
             << endl;
    }
}

string randomString(size_t length)
{
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?`~";
    static const size_t charsetSize = sizeof(charset) - 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, charsetSize - 1);

    string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
        result += charset[dis(gen)];

    return result;
}

int main()
{
    testBWT("banana", "Basic test with 'banana'");
    testBWT("", "Empty string test");
    testBWT("a", "Single character test");
    testBWT("aaaaaa", "All identical characters");
    testBWT("abcabcabcabc", "Repeating pattern");
    testBWT(randomString(50), "Random ASCII string (50 chars)");
    testBWT(randomString(1000), "Random ASCII string (1000 chars)");

    // Test with binary-like data
    string binaryData = "\x00\xFF\x01\x02\x03\x7F\x80\xAA\x55";
    testBWT(binaryData, "Binary-like data with non-printable chars");

    // Unicode test 
    testBWT("😀😁😂🤣😃😄😅😆😉😊", "Unicode emoji test (UTF-8)");

    cout << "All tests completed." << endl;
    return 0;
}
