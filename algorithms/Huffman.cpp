#include "Huffman.h"
#include <map>

HuffmanCoding::HuffmanCoding(){
    root = nullptr;
}
HuffmanCoding:: ~HuffmanCoding(){
    freeTree(root);
}

void HuffmanCoding :: buildHuffmanTree(const string &text){
    if(text.empty()){
        cerr << "[Error] Input text is empty . \n";
        return;
    }

    map<char , int> freq;
    for(char ch : text){
        freq[ch]++;
    }
    freqTable = freq; // <-- changed to map

    priority_queue<Node*  , vector <Node*> , Compare> pq;
    for(auto pair : freq){
        pq.push(new Node(pair.first , pair.second));
    }

    while(pq.size() > 1){
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *merged = new Node('\0' , left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    root = pq.top();
    buildCodes(root , "");
}

void HuffmanCoding::setFrequencyTable(const map<char, int>& table) {
    freqTable = table;
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freqTable) {
        pq.push(new Node(pair.first, pair.second));
    }
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        Node *merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }
    root = pq.top();
    codes.clear();
    reverseCodes.clear();
    buildCodes(root, "");
}

void HuffmanCoding::buildCodes(Node* node , string str){
    if(!node) return;

    if(!node->left && !node->right){
        codes[node->ch] = str;
        reverseCodes[str] = node->ch;
    }

    buildCodes(node->left , str+"0");
    buildCodes(node->right , str+"1");
}

string HuffmanCoding::encode(const string &text){
    if(text.empty()) return "";
    string encoded;
    for(char ch : text){
        encoded += codes[ch];
    }
    return encoded;
}

string HuffmanCoding::decode(const string &encodedStr){
    if(encodedStr.empty()) return "";

    string decoded;
    Node* current = root;

    for(char bit : encodedStr){
        if(bit == '0')
        current = current->left;
        else if(bit == '1')
        current= current->right;
        else throw runtime_error("Invalid encoded data.\n");

        if(!current->left && !current->right){
            decoded+= current->ch;
            current= root;
        }
    }
    return decoded;
}

bool HuffmanCoding::compressToFile(const string &inputText, const string &filename)
{
    if (inputText.empty())
    {
        cerr << "[Error] Cannot compress empty text.\n";
        return false;
    }

    ofstream outFile(filename, ios::binary);
    if (!outFile)
    {
        cerr << "[Error] Could not open file for writing.\n";
        return false;
    }

    saveFrequencyTable(outFile);

    string encoded = encode(inputText);
    unsigned char byte = 0;
    int bitCount = 0;

    for (char bit : encoded)
    {
        byte = (byte << 1) | (bit - '0'); 
        bitCount++;

        if (bitCount == 8)
        {
            outFile.put(byte);
            byte = 0;
            bitCount = 0;
        }
    }
    
    if (bitCount > 0)
    {
        byte <<= (8 - bitCount);
        outFile.put(byte);
    }

    outFile.close();
    size_t originalBits = inputText.size() * 8;
    size_t compressedBits = encoded.size(); 
    size_t compressedBytes = compressedBits / 8 + ((compressedBits % 8) ? 1 : 0);
    size_t totalFileSize = outFile.tellp(); 

    cout << "[Info] Original size: " << originalBits << " bits (" << originalBits / 8.0 << " bytes)\n";
    cout << "[Info] Compressed data size: " << compressedBits << " bits (" << compressedBytes << " bytes)\n";
    cout << "[Info] Total file size: " << totalFileSize << " bytes (includes tree + metadata)\n";
    cout << "[Info] Space saved: " << 100.0 * (1 - (double)totalFileSize / (originalBits / 8.0)) << "%\n";

    cout << "[Info] Compression successful. Saved to " << filename << endl;
    cout << "[Info] Compression ratio: "<< (inputText.size() * 8.0) / encoded.size() << endl;
    return true;
}

bool HuffmanCoding::decompressFromFile(const string &filename, string &outputText)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile)
    {
        cerr << "[Error] Could not open file for reading.\n";
        return false;
    }

    loadFrequencyTable(inFile);

    string encoded = "";
    char byte;
    while (inFile.get(byte))
    {
        for (int i = 7; i >= 0; --i)
        {
            encoded += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    inFile.close();

    try
    {
        outputText = decode(encoded);
    }
    catch (exception &e)
    {
        cerr << "[Error] " << e.what() << endl;
        return false;
    }

    cout << "[Info] Decompression successful.\n";
    return true;
}

void HuffmanCoding::freeTree(Node* node){
    if(!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void HuffmanCoding::saveFrequencyTable(ofstream &outFile)
{
    outFile << codes.size() << '\n'; 
    for (const auto &pair : codes)
    {
        outFile << (int)pair.first << ' ' << pair.second << '\n';
    }
}

void HuffmanCoding::loadFrequencyTable(ifstream &inFile)
{
    int n;
    inFile >> n; 
    inFile.ignore();

    char ch;
    string code;
    for (int i = 0; i < n; ++i)
    {
        int ascii;
        inFile >> ascii >> code;
        inFile.ignore();
        ch = (char)ascii;
        codes[ch] = code;
        reverseCodes[code] = ch;
    }
}
