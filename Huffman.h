#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

struct Node
{
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};

class HuffmanCoding
{

private:
    Node *root;
    unordered_map<char, string> codes;
    unordered_map<string, char> reverseCodes;

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
};

#endif