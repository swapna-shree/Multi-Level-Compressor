#include "Compressor.cpp"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
    Compressor compressor;
    string input;
    getline(cin, input);
    
    // Check if input is JSON (decompression) or plain text (compression)
    if (input.find("\"compressed\":") != string::npos) {
        // Decompression mode
        size_t compressedStart = input.find("\"compressed\":\"") + 14;
        size_t compressedEnd = input.find("\"", compressedStart);
        string compressed = input.substr(compressedStart, compressedEnd - compressedStart);
        
        size_t indexStart = input.find("\"primaryIndex\":") + 15;
        size_t indexEnd = input.find("}", indexStart);
        int primaryIndex = stoi(input.substr(indexStart, indexEnd - indexStart));
        
        string decompressed = compressor.decompress(compressed, primaryIndex);
        cout << decompressed << endl;
    } else {
        // Compression mode
        string compressed = compressor.compress(input);
        int primaryIndex = compressor.getPrimaryIndex();
        
        // Output JSON format
        cout << "{\"compressed\":\"" << compressed << "\",\"primaryIndex\":" << primaryIndex << "}" << endl;
    }
    
    return 0;
} 