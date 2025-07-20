#include "Compressor.cpp"
#include <iostream>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <sstream>
using namespace std;

string escape_json(const string& s) {
    ostringstream out;
    for (unsigned char c : s) {
        switch (c) {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:
                if (c < 0x20 || c > 0x7E) {
                    out << "\\u" << hex << setw(4) << setfill('0') << (int)c;
                } else {
                    out << c;
                }
        }
    }
    return out.str();
}

int main() {
    Compressor compressor;
    string input, line;
    while (getline(cin, line)) {
        input += line;
        input += '\n';
    }
    if (!input.empty() && input.back() == '\n') input.pop_back();
    string compressed = compressor.compress(input);
    int primaryIndex = compressor.getPrimaryIndex();
    auto freqTable = compressor.getHuffman().getFrequencyTable();
    cout << "{\"compressed\":\"" << escape_json(compressed) << "\",\"primaryIndex\":" << primaryIndex << ",\"freqTable\":{";
    bool first = true;
    for (auto& kv : freqTable) {
        if (!first) cout << ",";
        cout << "\"" << (int)(unsigned char)kv.first << "\":" << kv.second;
        first = false;
    }
    cout << "}}" << endl;
    return 0;
} 