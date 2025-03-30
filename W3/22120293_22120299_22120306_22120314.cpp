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

const int IP_table[64] = {58, 50, 42, 34, 26, 18, 10, 2, // intital permutation table
                          60, 52, 44, 36, 28, 20, 12, 4,
                          62, 54, 46, 38, 30, 22, 14, 6,
                          64, 56, 48, 40, 32, 24, 16, 8,
                          57, 49, 41, 33, 25, 17, 9, 1,
                          59, 51, 43, 35, 27, 19, 11, 3,
                          61, 53, 45, 37, 29, 21, 13, 5,
                          63, 55, 47, 39, 31, 23, 15, 7};

const int E_Table[48] = {32, 1, 2, 3, 4, 5, // expantion table
                         4, 5, 6, 7, 8, 9,
                         8, 9, 10, 11, 12, 13,
                         12, 13, 14, 15, 16, 17,
                         16, 17, 18, 19, 20, 21,
                         20, 21, 22, 23, 24, 25,
                         24, 25, 26, 27, 28, 29,
                         28, 29, 30, 31, 32, 1};

int S[8][4][16] = { // S-box
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

const int P[32] = {16, 7, 20, 21,
                   29, 12, 28, 17,
                   1, 15, 23, 26,
                   5, 18, 31, 10,
                   2, 8, 24, 14,
                   32, 27, 3, 9,
                   19, 13, 30, 6,
                   22, 11, 4, 25};

const int FP_table[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                          39, 7, 47, 15, 55, 23, 63, 31,
                          38, 6, 46, 14, 54, 22, 62, 30,
                          37, 5, 45, 13, 53, 21, 61, 29,
                          36, 4, 44, 12, 52, 20, 60, 28,
                          35, 3, 43, 11, 51, 19, 59, 27,
                          34, 2, 42, 10, 50, 18, 58, 26,
                          33, 1, 41, 9, 49, 17, 57, 25};

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
        for (int i = 7; i >= 0; --i)
        {
            key_binary += ((ch >> i) & 1) ? '1' : '0';
        }
    }
    return key_binary;
}

void binaryStringToBytes(const string &binaryString, unsigned char *bytes, const int &byteLength = 6)
{
    for (int i = 0; i < byteLength; i++)
    {
        unsigned char value = 0;
        for (int j = 0; j < 8; j++)
        {
            value = (value << 1) | (binaryString[i * 8 + j] - '0');
        }
        bytes[i] = static_cast<unsigned char>(value);
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

    int len = getStrLength(bin_key);

    string C = bin_key.substr(0, len / 2);
    string D = bin_key.substr(len / 2);

    for (int r = 0; r < 16; r++)
    {
        C = LCS(C, SHIFT_SCHEDULE[r]);
        D = LCS(D, SHIFT_SCHEDULE[r]);

        string subkey = permute_PC2(C + D);

        Keys.push_back(subkey);
    }
    return Keys;
}

string XOR(const string &a, const string &b)
{
    string output = "";
    int len = getStrLength(a);
    for (int i = 0; i < len; i++)
    {
        output += (a[i] == b[i]) ? '0' : '1';
    }
    return output;
}

string expansion(const string &key_32)
{
    string output = "";
    for (int i = 0; i < 48; i++)
    {
        output += key_32[E_Table[i] - 1];
    }
    return output;
}

string permutation(const string &key)
{
    string output = "";
    for (int i = 0; i < 32; i++)
    {
        output += key[P[i] - 1];
    }
    return output;
}

// Chia 48-bit thành 8 khối 6-bit, mỗi khối qua một S-box riêng → Đầu ra 4-bit/khối.
string sBoxSubstitution(const string &key)
{
    string output = "";
    for (int i = 0; i < 8; i++)
    {
        string subkey_6bit = key.substr(i * 6, 6);
        // Xác định hàng: bit đầu và bit cuối
        int row = (subkey_6bit[0] - '0') * 2 + (subkey_6bit[5] - '0');

        // Xác định cột: 4 bit giữa
        int col = 0;
        for (int j = 1; j <= 4; j++)
        {
            col = (col << 1) | (subkey_6bit[j] - '0');
        }

        // Lấy giá trị từ S-box và chuyển thành 4-bit
        int val = S[i][row][col];
        output += bitset<4>(val).to_string();
    }
    return output;
}

string f(const string &R, const string &K)
{
    // Bước 1: Mở rộng 32-bit → 48-bit
    string expanded = expansion(R);

    // Bước 2: XOR với khóa con 48-bit
    string xored = XOR(expanded, K);

    // Bước 3: Thay thế qua S-boxes (48-bit → 32-bit)
    string substituted = sBoxSubstitution(xored);

    // Bước 4: Hoán vị 32-bit
    string permuted = permutation(substituted);

    return permuted;
}

string permute_IP(const string &key_64)
{
    string output;
    for (int i = 0; i < 64; i++)
    {
        output += key_64[IP_table[i] - 1];
    }
    return output;
}

string permute_FP(const string &key_32)
{
    string output;
    for (int i = 0; i < 64; i++)
    {
        output += key_32[FP_table[i] - 1];
    }
    return output;
}

string Encrypt(const string &Key, const string &message)
{
    vector<string> Keys = CreateSubKey(Key);

    for (int k = 0; k < 16; k++)
    {
        unsigned char subkey[6]; // chỗ này lưu subkey 6x8 bits
        binaryStringToBytes(Keys[k], subkey, 6);

        printf("SubKey %d:\t", k);
        for (int i = 0; i < 6; i++)
        {
            printf_s("%02x ", subkey[i]);
        }
        printf("\n");
    }

    string M_bin = StrToBinary(message);

    // cout << M_bin << endl;
    string ciphertext;

    string block = permute_IP(M_bin);
    string L = block.substr(0, 32);
    string R = block.substr(32);

    // cout << L.length() << " - " << R.length() << endl;
    for (int round = 0; round < 16; round++)
    {
        string temp = R;
        R = XOR(L, f(R, Keys[round]));
        L = temp;
    }

    ciphertext += permute_FP(L + R);

    return ciphertext;
}

int main()
{
    string Key = "abcdefgh";
    string message = "YuSato!!";
    // vector<string> Keys = CreateSubKey(Key);

    string ciphertext = Encrypt(Key, message);

    printf_s("Encrypted:\t%02x ", ciphertext);

    return 0;
}
