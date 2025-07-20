#include "BWT.h"
#include <vector>
#include <algorithm>

using namespace std;

string bwtEncode(const string &input, int &primaryIndex)
{
    int n = input.size();
    vector<int> suffixArray(n);
    for (int i = 0; i < n; i++)
        suffixArray[i] = i;

    // Efficient cyclic comparator
    sort(suffixArray.begin(), suffixArray.end(), [&](int a, int b)
         {
             for (int i = 0; i < n; ++i)
             {
                 unsigned char ca = input[(a + i) % n];
                 unsigned char cb = input[(b + i) % n];
                 if (ca != cb)
                     return ca < cb;
             }
             return false; 
         });

    string bwt;
    bwt.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int index = suffixArray[i];
        bwt += (index == 0) ? input[n - 1] : input[index - 1];
        if (index == 0)
            primaryIndex = i;
    }
    return bwt;
}

string bwtDecode(const string &bwtString, int primaryIndex)
{
    if (bwtString.empty())
        return "";

    int n = bwtString.size();
    vector<int> count(256, 0);
    vector<int> tots(256, 0);

    // Count character occurrences
    for (char ch : bwtString)
        count[(unsigned char)ch]++;

    // Compute totals
    for (int i = 1; i < 256; i++)
        tots[i] = tots[i - 1] + count[i - 1];

    // Build LF-mapping
    vector<int> lf(n);
    vector<int> occ(256, 0);
    for (int i = 0; i < n; i++)
    {
        lf[i] = tots[(unsigned char)bwtString[i]] + occ[(unsigned char)bwtString[i]];
        occ[(unsigned char)bwtString[i]]++;
    }

    // Reconstruct original string
    string result(n, ' ');
    int index = primaryIndex;
    for (int i = n - 1; i >= 0; i--)
    {
        result[i] = bwtString[index];
        index = lf[index];
    }
    return result;
}
