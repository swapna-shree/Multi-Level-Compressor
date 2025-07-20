#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

struct Node
{
    unsigned char ch;
    int freq;
    Node *left, *right;

    Node(unsigned char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        if (a->freq == b->freq)
            return a->ch > b->ch; // break ties by character value
        return a->freq > b->freq;
    }
};

class HuffmanCoding
{

private:
    Node *root;
    unordered_map<unsigned char, string> codes;
    unordered_map<string, unsigned char> reverseCodes;
    map<unsigned char, int> freqTable;

    void buildCodes(Node *node, string str);
    void freeTree(Node *node);
    void saveFrequencyTable(ofstream &outFile);
    void loadFrequencyTable(ifstream &inFile);

public:
    HuffmanCoding();
    ~HuffmanCoding();

    void buildHuffmanTree(const string &text);
    string encode(const string &text);
    string decode(const string &encodedStr);

    bool compressToFile(const string &inputText, const string &filename);
    bool decompressFromFile(const string &filename, string &outputText);

    map<unsigned char, int> getFrequencyTable() const { return freqTable; }
    void setFrequencyTable(const map<unsigned char, int>& table);
};

#endif