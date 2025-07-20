#include "BWT.h"
#include "MTF.h"
#include "RLE.h"
#include "Huffman.h"
#include <iostream>
#include <string>
#include<fstream>

using namespace std;

class Compressor
{
private:
    int primaryIndex; 
    HuffmanCoding huffman;

public:
    
    string compress(const string &input)
    {
        cout << "\n========== Compression Pipeline ==========" << endl;
        string bwtResult = bwtEncode(input, primaryIndex);
        cout << "[1/4] BWT Transform        ... Done (Primary Index: " << primaryIndex << ")" << endl;
        cout << "[2/4] Move-To-Front (MTF)  ... ";
        string mtfResult = mtfEncode(bwtResult);
        cout << "Done" << endl;
        cout << "[3/4] Run-Length Encoding  ... ";
        string rleResult = rleEncoded(mtfResult);
        cout << "Done" << endl;
        cout << "[4/4] Huffman Coding       ... ";
        huffman.buildHuffmanTree(rleResult);
        string compressedData = huffman.encode(rleResult);
        cout << "Done" << endl;
        cout << "==========================================" << endl;
        cout << "[Info] Compression Primary Index: " << primaryIndex << endl;
        cout << "==========================================\n" << endl;
        return compressedData;
    }

    // Decompress input string using the pipeline
    string decompress(const string &compressed, int primaryIndex)
    {
        cerr << "\n======== Decompression Pipeline =========" << endl;
        cerr << "[Info] Using Primary Index: " << primaryIndex << endl;
        cerr << "[1/4] Huffman Decoding     ... ";
        string rleResult = huffman.decode(compressed);
        cerr << "Done" << endl;
        cerr << "[2/4] RLE Decoding         ... ";
        string mtfResult = rleDecoded(rleResult);
        cerr << "Done" << endl;
        cerr << "[3/4] MTF Decoding         ... ";
        string bwtResult = mtfDecode(mtfResult);
        cerr << "Done" << endl;
        cerr << "[4/4] BWT Inverse Transform... ";
        string originalText = bwtDecode(bwtResult, primaryIndex);
        cerr << "Done" << endl;
        cerr << "==========================================" << endl;
        cerr << "[Info] Decompression Primary Index: " << primaryIndex << endl;
        cerr << "==========================================\n" << endl;
        return originalText;
    }

    int getPrimaryIndex() const
    {
        return primaryIndex;
    }

    HuffmanCoding& getHuffman() { return huffman; }
    void setHuffman(const HuffmanCoding& h) { huffman = h; }
};

