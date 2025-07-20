#include "Compressor.cpp"
#include <iostream>
#include <sstream>
#include <map>
using namespace std;

int main() {
    Compressor compressor;
    string input, line;
    while (getline(cin, line)) {
        input += line;
    }
    // Debug: print the raw input
    cerr << "[DEBUG] Raw input: " << input << endl;

    size_t compressedStart = input.find("\"compressed\":\"");
    if (compressedStart == string::npos) {
        cerr << "[ERROR] Could not find compressed data in input!" << endl;
        return 1;
    }
    compressedStart += 14;
    size_t compressedEnd = input.find("\"", compressedStart);
    if (compressedEnd == string::npos) {
        cerr << "[ERROR] Could not find end of compressed data!" << endl;
        return 1;
    }
    string compressed = input.substr(compressedStart, compressedEnd - compressedStart);

    size_t indexStart = input.find("\"primaryIndex\":");
    if (indexStart == string::npos) {
        cerr << "[ERROR] Could not find primaryIndex in input!" << endl;
        return 1;
    }
    indexStart += 15;
    size_t indexEnd = input.find(",", indexStart);
    if (indexEnd == string::npos) indexEnd = input.find("}", indexStart);
    int primaryIndex = stoi(input.substr(indexStart, indexEnd - indexStart));

    size_t freqStart = input.find("\"freqTable\":{");
    if (freqStart == string::npos) {
        cerr << "[ERROR] Could not find freqTable in input!" << endl;
        return 1;
    }
    freqStart += 12;
    size_t freqEnd = input.find("}", freqStart);
    if (freqEnd == string::npos) {
        cerr << "[ERROR] Could not find end of freqTable!" << endl;
        return 1;
    }
    string freqStr = input.substr(freqStart, freqEnd - freqStart);

    map<char, int> freqTable;
    stringstream ss(freqStr);
    string pair;
    while (getline(ss, pair, ',')) {
        size_t colon = pair.find(':');
        if (colon != string::npos) {
            size_t quote = pair.find('"');
            if (quote == string::npos || quote+1 >= pair.size()) continue;
            int key = stoi(pair.substr(quote+1, colon-quote-1));
            int value = stoi(pair.substr(colon+1));
            freqTable[(char)key] = value;
        }
    }

    // Debug output
    cerr << "[DEBUG] Compressed: " << compressed << endl;
    cerr << "[DEBUG] Compressed length: " << compressed.length() << endl;
    cerr << "[DEBUG] Primary Index: " << primaryIndex << endl;
    cerr << "[DEBUG] Frequency Table:" << endl;
    for (auto& kv : freqTable) {
        cerr << "  '" << kv.first << "': " << kv.second << endl;
    }

    compressor.getHuffman().setFrequencyTable(freqTable);

    string decompressed = compressor.decompress(compressed, primaryIndex);
    cout << decompressed << endl;
    return 0;
} 