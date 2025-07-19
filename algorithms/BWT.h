#pragma once

#include<string>
using namespace std;

string bwtEncode(const string &input , int &primaryIndex);
string bwtDecode(const string &bwtString , int primaryIndex);