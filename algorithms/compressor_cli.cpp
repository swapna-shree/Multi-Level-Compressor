#include "Compressor.cpp"
#include <iostream>
using namespace std;

int main() {
    Compressor compressor;
    string input;
    getline(cin, input); 
    string compressed = compressor.compress(input);
    int primaryIndex = compressor.getPrimaryIndex();
    cout << "{\"compressed\":\"" << compressed << "\",\"primaryIndex\":" << primaryIndex << "}" << endl;
    return 0;
} 