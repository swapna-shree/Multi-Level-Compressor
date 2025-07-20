#include "Compressor.cpp"
#include <iostream>
#include <unordered_map>
#include <map>
using namespace std;

int main() {
    Compressor compressor;
    string input;
    getline(cin, input);
    string compressed = compressor.compress(input);
    int primaryIndex = compressor.getPrimaryIndex();
    auto freqTable = compressor.getHuffman().getFrequencyTable();
    cout << "{\"compressed\":\"" << compressed << "\",\"primaryIndex\":" << primaryIndex << ",\"freqTable\":{";
    bool first = true;
    for (auto& kv : freqTable) {
        if (!first) cout << ",";
        cout << "\"" << (int)(unsigned char)kv.first << "\":" << kv.second;
        first = false;
    }
    cout << "}}" << endl;
    return 0;
} 