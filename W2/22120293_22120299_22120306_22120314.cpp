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

string exchange_table(string str)
{
    int numCols = keyword.length();
    while (str.length() % numCols != 0)
        str += ' ';

    int numRows = str.length() / numCols;

    vector<vector<char>> table(numRows, vector<char>(numCols));
    for (int i = 0; i < str.length(); i++)
    {
        table[i / numCols][i % numCols] = str[i];
    }

    vector<int> col_index(numCols);
    for (int i = 0; i < numCols; i++)
        col_index[i] = i;

    sort(col_index.begin(), col_index.end(), [&](int a, int b)
         { return keyword[a] < keyword[b]; });

    string ciphertexts = "";
    for (int i : col_index)
        for (int j = 1; j < numRows; j++)
            ciphertexts += table[j][i];

    return ciphertexts;
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

    string Ciphertexts = keyword + ciphertexts_temp;

    return exchange_table(Ciphertexts);
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