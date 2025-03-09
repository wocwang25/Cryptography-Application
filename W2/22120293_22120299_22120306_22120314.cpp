#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
using namespace std;

string keyword = "MARK", title = "HCMUNS";
int rows = 6, cols = 6;
int add_char = 0;
char table[6][6] = {
    {'8', 'p', '3', 'd', '1', 'n'},
    {'l', 't', '4', 'o', 'a', 'h'},
    {'7', 'k', 'b', 'c', '5', 'z'},
    {'j', 'u', '6', 'w', 'g', 'm'},
    {'x', 's', 'v', 'i', 'r', '2'},
    {'9', 'e', 'y', '0', 'f', 'q'}};

unordered_map<int, char> mapping(const string &title, bool reversed = true)
{
    unordered_map<int, char> map_char;
    int i = 0;
    if (reversed)
    {
        for (char ch : title)
        {
            map_char.insert({i++, ch});
        }
    }
    else
    {
        for (char ch : title)
        {
            map_char.insert({ch, i++});
        }
    }
    return map_char;
}

unordered_map<int, char> map_char = mapping(title);
unordered_map<int, char> map_int = mapping(title, false);

vector<vector<char>> create_table(string &str, bool encrypt = true)
{
    int numCols = keyword.length();
    while (str.length() % numCols != 0)
    {
        str += 'S';
    }

    int numRows = str.length() / numCols;

    vector<vector<char>> table(numRows, vector<char>(numCols, ' '));
    if (encrypt)
    {
        int char_index = 0;
        for (int row = 0; row < numRows; row++)
        {
            // table[i / numCols][i % numCols] = str[i]; // tạo bảng theo dòng -> encrypt
            for (int col = 0; col < numCols; col++)
            {
                table[row][col] = str[char_index++];
            }
        }
    }
    else
    {
        for (int col = 0; col < numCols; col++)
        {
            for (int row = 0; row < numRows; row++)
            {
                table[row][col] = str[row + col * numRows]; // tạo bảng theo cột -> decrypt
            }
        }
    }

    return table;
}

void print_table(const vector<vector<char>> &table)
{
    for (const auto &row : table)
    {
        for (char ch : row)
        {
            cout << ch << " ";
        }
        cout << endl;
    }
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
                    ciphertexts_temp += map_char[i];
                    ciphertexts_temp += map_char[j];
                    // cout << ch << "-" << map_char[i] << "-" << map_char[j] << "\n";
                }
            }
        }
    }

    // string table_data = ciphertexts_temp;
    // cout << ciphertexts_temp.length() << endl;
    add_char = ciphertexts_temp.length();
    vector<vector<char>> table_enc = create_table(ciphertexts_temp);

    // print_table(table_enc);

    int numCols = keyword.length();
    int numRows = ciphertexts_temp.length() / numCols;

    vector<int> col_order(numCols);
    for (int i = 0; i < numCols; i++)
        col_order[i] = i;

    sort(col_order.begin(), col_order.end(), [&](int a, int b)
         { return keyword[a] < keyword[b]; });

    string ciphertexts = "";
    // cout << "Rows: " << numRows << " - Cols: " << numCols << endl;
    for (int col : col_order)
    {
        for (int row = 0; row < numRows; row++)
        {
            ciphertexts += table_enc[row][col];
            // cout << table_enc[row][col] << "-";
        }
    }
    // cout << endl;

    return ciphertexts;
}

void reorderRows(vector<vector<char>> &table, const vector<int> &col_order)
{
    for (auto &row : table)
    {
        vector<pair<char, int>> row_with_index;

        for (int col = 0; col < row.size(); col++)
        {
            row_with_index.push_back({row[col], col_order[col]});
        }

        sort(row_with_index.begin(), row_with_index.end(),
             [](const pair<char, int> &a, const pair<char, int> &b)
             {
                 return a.second < b.second;
             });

        for (int col = 0; col < row.size(); col++)
        {
            row[col] = row_with_index[col].first;
        }
    }
}

string decryptXXXXXX(string ciphertexts)
{
    int numCols = keyword.length();
    int numRows = ciphertexts.length() / numCols;

    vector<int> col_order(numCols);
    for (int i = 0; i < numCols; i++)
        col_order[i] = i;

    sort(col_order.begin(), col_order.end(), [](int &a, int &b)
         { return keyword[a] < keyword[b]; });

    vector<vector<char>> table_enc = create_table(ciphertexts, false);
    // print_table(table_enc);

    reorderRows(table_enc, col_order);
    // cout << endl;
    // print_table(table_enc);

    string plaintext_raw = "";
    for (int row = 0; row < numRows; row++)
    {
        for (int col = 0; col < numCols; col++)
        {
            if (plaintext_raw.length() < add_char)
            {
                plaintext_raw += table_enc[row][col];
            }
        }
    }

    // cout << "\nResults: " << plaintext_raw << endl;

    int x = 0, y = 0;

    string decryptedtexts = "";

    for (int i = 0; i < plaintext_raw.length() - 1; i += 2)
    {
        char char_1 = plaintext_raw[i];
        char char_2 = plaintext_raw[i + 1];
        x = map_int[char_1];
        y = map_int[char_2];
        // cout << char_1 << "-" << char_2 << "->" << x << "_" << y << "->";

        decryptedtexts += table[x][y];
        // cout << decryptedtexts << "\n";
    }

    return decryptedtexts;
}

int main()
{
    string plaintexts = "attack at 10 pmm";
    string ciphertexts = encryptXXXXXX(plaintexts);
    cout << "Results: " << ciphertexts << endl;
    string i = "NCUNNCCNCCUSCCMCHHCCMCSU";

    string decryptedtexts = decryptXXXXXX(ciphertexts);
    cout << "Results: " << decryptedtexts << endl;
}
