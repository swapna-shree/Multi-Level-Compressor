#include "MTF.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

void testMTF(const string &input, const string &desc)
{
    cout << "[Test] " << desc << endl;
    vector<int> encoded = mtfEncode(input);
    string decoded = mtfDecode(encoded);

    if (decoded == input)
    {
        cout << "Passed\n";
    }
    else
    {
        cout << "Failed\n";
        cout << "Expected: " << input << "\nGot:      " << decoded << endl;
    }
}

int main()
{
    testMTF("banana", "Simple word test");
    testMTF("", "Empty string test");
    testMTF("a", "Single character test");
    testMTF(string(1000, 'A'), "Long repeat test (1000x 'A')");
    testMTF("\x00\xFF\x7F\x01\x00", "Non-printable & binary-like chars");
    testMTF("abcABC123!@#", "Mixed symbols test");
    testMTF("😀😂😅😉", "UTF-8 Emoji test (if supported)");

    cout << "All tests completed." << endl;
    return 0;
}
