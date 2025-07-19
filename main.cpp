#include "Huffman.h"

int main(){
    HuffmanCoding hc;

    string text;
    cout << "Enter text to compress : "<< endl;
    getline(cin , text);

    hc.buildHuffmanTree(text);

    if(!hc.compressToFile(text ,"compressed.huff")) return 1;

    string decompressedText;
    if(!hc.decompressFromFile("compressed.huff" , decompressedText)) return 1;

    cout << "Decompressed Text : " << decompressedText << endl;

    return 0;
}