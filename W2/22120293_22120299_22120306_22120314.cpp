#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

string keyword = "MARK", title = "HCMUNS";
int rows = 6, cols = 6;
char table[6][6] = {
    {'8', 'p', '3', 'd', '1', 'n'},
    {'l', 't', '4', 'o', 'a', 'h'},
    {'7', 'k', 'b', 'c', '5', 'z'},
    {'j', 'u', '6', 'w', 'g', 'm'},
    {'x', 's', 'v', 'i', 'r', '2'},
    {'9', 'e', 'y', '0', 'f', 'q'}};

map<int, char> mapping(const string &title)
{
    map<int, char> map_char;
    int i = 1;
    for (char ch : title)
    {
        map_char.insert({i++, ch});
    }
    return map_char;
}

string encryptXXXXXX(string plaintexts)
{
    string ciphertexts_temp = "";
    string temp_str = plaintexts;
    for (char ch : temp_str)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (ch == table[i][j])
                {
                    ciphertexts_temp += mapping(title)[i + 1];
                    ciphertexts_temp += mapping(title)[j + 1];
                }
            }
        }
    }

    int numCols = keyword.length();
    string Ciphertexts = keyword + ciphertexts_temp;

    return ciphertexts_temp;
}

int main()
{
    // char table[6][6] = {
    //     {'8', 'p', '3', 'd', '1', 'n'},
    //     {'l', 't', '4', 'o', 'a', 'h'},
    //     {'7', 'k', 'b', 'c', '5', 'z'},
    //     {'j', 'u', '6', 'w', 'g', 'm'},
    //     {'x', 's', 'v', 'i', 'r', '2'},
    //     {'9', 'e', 'y', '0', 'f', 'q'}};

    string plaintexts = "attack at 10 pm";
    string ciphertexts = encryptXXXXXX(plaintexts);
    cout << "Results: " << ciphertexts << endl;

    // string decryptedtexts = decryptXXXXXX(ciphertexts);
    // cout << "Results: " << decryptedtexts << endl;
}