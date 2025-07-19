#include "Compressor.cpp"
#include <iostream>
#include <sstream>
using namespace std;

int main() {
    Compressor compressor;
    string input;
    getline(cin, input);
    
    // Simple JSON parsing (for demo purposes)
    // Expected format: {"compressed":"...","primaryIndex":123}
    size_t compressedStart = input.find("\"compressed\":\"") + 14;
    size_t compressedEnd = input.find("\"", compressedStart);
    string compressed = input.substr(compressedStart, compressedEnd - compressedStart);
    
    size_t indexStart = input.find("\"primaryIndex\":") + 15;
    size_t indexEnd = input.find("}", indexStart);
    int primaryIndex = stoi(input.substr(indexStart, indexEnd - indexStart));
    
    string decompressed = compressor.decompress(compressed, primaryIndex);
    cout << decompressed << endl;
    return 0;
} 