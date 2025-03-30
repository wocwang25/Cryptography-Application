#include <iostream>
#include <string>
#include <cctype>

using namespace std;

string generateKey(const string& text, const string& key) {
    string extendedKey = key;
    int keyLength = key.length();
    int textLength = text.length();

    for (int i = 0; extendedKey.length() < textLength; ++i) {
        extendedKey += key[i % keyLength];
    }
    return extendedKey;
}

string encrypt(const string& text, const string& key) {
    string cipherText = "";
    string extendedKey = generateKey(text, key);

    for (int i = 0; i < text.length(); ++i) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            int shift = tolower(extendedKey[i]) - 'a';
            char encryptedChar = (text[i] - base + shift) % 26 + base;
            cipherText += encryptedChar;
        }
        else {
            cipherText += text[i]; 
        }
    }
    return cipherText;
}

string decrypt(const string& cipherText, const string& key) {
    string originalText = "";
    string extendedKey = generateKey(cipherText, key);

    for (int i = 0; i < cipherText.length(); ++i) {
        if (isalpha(cipherText[i])) {
            char base = isupper(cipherText[i]) ? 'A' : 'a';
            int shift = tolower(extendedKey[i]) - 'a';
            char decryptedChar = (cipherText[i] - base - shift + 26) % 26 + base;
            originalText += decryptedChar;
        }
        else {
            originalText += cipherText[i]; 
        }
    }
    return originalText;
}

int main() 
{

    string text = "ABCDEFGHIJKL";
    string key = "KEYWORD";

    string cipherText = encrypt(text, key);
    cout << "Encrypted text: " << cipherText << endl;

    string decryptedText = decrypt(cipherText, key);
    cout << "Decrypted text: " << decryptedText << endl;

    return 0;
}
