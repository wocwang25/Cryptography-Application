#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <bitset>
using namespace std;

static const int SHIFT_SCHEDULE[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1};

static const int PC1_Table[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

static const int PC2_Table[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

int getStrLength(const string &str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}
string StrToBinary(const string &str)
{
    string key_binary = "";

    for (unsigned char ch : str)
    {
        key_binary += bitset<8>(ch).to_string();
    }
    return key_binary;
}

void binaryStringToBytes(const string &binaryString, unsigned char *bytes, const int &byteLength)
{
    for (int i = 0; i < byteLength; i++)
    {
        bitset<8> bit(binaryString.substr(i * 8, 8));
        bytes[i] = (unsigned char)bit.to_ulong();
    }
}

bool ExtractBit(string bin_str, unsigned int idx)
{
    return int(bin_str[idx - 1]);
}

void ReplaceBit(string &bin_str, unsigned int idx, bool val)
{
    if (val)
        bin_str[idx - 1] = 1; // Set bit 1
    else
        bin_str[idx - 1] = 0; // Set bit 0
}

string LCS(string &subkey, int shift)
{
    string temp = subkey.substr(0, shift);
    string key = subkey.substr(shift) + temp;
    return key;
}

string permute_PC1(const string &bin_key, const int PC1_temp[])
{
    string output(56, '0');
    for (int i = 0; i < 56; i++)
    {
        int index = PC1_Table[i] - (PC1_Table[i] / 8);
        output[i] = bin_key[index - 1];
    }
    return output;
}

// Hàm hoán vị PC-2
string permute_PC2(const string &permuted_key)
{
    string subkey = "";
    for (int i = 0; i < 48; i++)
    {
        subkey += permuted_key[PC2_Table[i] - 1];
    }
    return subkey;
}

vector<string> CreateSubKey(const string &Key)
{
    vector<string> Keys;
    int PC1_temp[56] = {0};

    string bin_key = StrToBinary(Key);
    bin_key = permute_PC1(bin_key, PC1_temp);

    string C = bin_key.substr(0, 28);
    string D = bin_key.substr(28);

    for (int r = 0; r < 16; r++)
    {
        C = LCS(C, SHIFT_SCHEDULE[r]);
        D = LCS(D, SHIFT_SCHEDULE[r]);

        string subkey = permute_PC2(C + D);

        Keys.push_back(subkey);
    }
    return Keys;
}

int main()
{
    string Key = "abcdefgh";
    vector<string> Keys = CreateSubKey(Key);

    for (int k = 0; k < 16; k++)
    {
        printf("SubKey %d:\t", k);
        unsigned char key[6];
        binaryStringToBytes(Keys[k], key, 6);
        for (int i = 0; i < 6; i++)
        {
            printf_s("%02x ", key[i]);
        }
        printf("\n");
    }

    return 0;
}
