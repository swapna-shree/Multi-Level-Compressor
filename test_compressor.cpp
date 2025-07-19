#include "BWT.h"
#include "MTF.h"
#include "RLE.h"
#include "Huffman.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string compressPipeline(const string &input, int &primaryIndex)
{
    // Step 1: BWT
    string bwtResult = bwtEncode(input, primaryIndex);
    cout << "[BWT] Output size: " << bwtResult.size() << endl;

    // Step 2: MTF
    string mtfResult = mtfEncode(bwtResult);
    cout << "[MTF] Output size: " << mtfResult.size() << endl;

    // Step 3: RLE
    string rleResult = rleEncoded(mtfResult);
    cout << "[RLE] Output size: " << rleResult.size() << endl;

    // Step 4: Huffman
    HuffmanCoding huffman;
    huffman.buildHuffmanTree(rleResult);
    string huffmanResult = huffman.encode(rleResult);
    cout << "[Huffman] Output size: " << huffmanResult.size() << endl;

    return huffmanResult;
}

string decompressPipeline(const string &compressed, int primaryIndex)
{
    // Step 1: Huffman Decode
    HuffmanCoding huffman;
    string rleResult = huffman.decode(compressed);
    cout << "[Huffman Decode] Size: " << rleResult.size() << endl;

    // Step 2: RLE Decode
    string mtfResult = rleDecoded(rleResult);
    cout << "[RLE Decode] Size: " << mtfResult.size() << endl;

    // Step 3: MTF Decode
    string bwtResult = mtfDecode(mtfResult);
    cout << "[MTF Decode] Size: " << bwtResult.size() << endl;

    // Step 4: BWT Decode
    string original = bwtDecode(bwtResult, primaryIndex);
    cout << "[BWT Decode] Size: " << original.size() << endl;

    return original;
}

void runTests()
{
    vector<string> testCases = {
        "banana",
        "aaaaaaa",
        "abcabcabcabcabcabc",
        "The quick brown fox jumps over the lazy dog!",
        "1234567890!@#$%^&*()_+-=[]{}|;:',.<>/?",
        string(1000, 'x'),                                         // long single character
        "ab" + string(500, 'c') + "de",                            // large repeat in middle
        string(5000, 'A') + string(5000, 'B') + string(5000, 'C'), // very large block repeats
        string(10000, 'Z')                                         // extremely large input
    };

    for (size_t i = 0; i < testCases.size(); ++i)
    {
        const string &test = testCases[i];
        cout << "\n=== Test Case " << i + 1 << " ===" << endl;
        if (test.size() < 100)
        {
            cout << "Input: " << test << endl;
        }
        else
        {
            cout << "Input: [Large string of size " << test.size() << "]" << endl;
        }

        int primaryIndex = 0;
        string compressed = compressPipeline(test, primaryIndex);
        cout << "[Info] Compression complete. Primary Index: " << primaryIndex << endl;

        string decompressed = decompressPipeline(compressed, primaryIndex);
        cout << "[Info] Decompression complete." << endl;

        if (decompressed == test)
        {
            cout << "Test passed: decompressed string matches original." << endl;
        }
        else
        {
            cout << "Test failed: decompressed string does not match original." << endl;
        }
    }
}

int main()
{
    cout << "Starting advanced pipeline tests..." << endl;
    runTests();
    cout << "All tests completed." << endl;
    return 0;
}
