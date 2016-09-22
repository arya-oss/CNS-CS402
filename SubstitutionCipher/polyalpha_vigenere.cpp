/**
 * Polyalphabatic Cipher
 * Vigenere Cipher
 *
 * Author: Rajmani Arya
 * Date: 31st Aug 2016
 */


#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc, char * argv[]) {
    
    string key, text, cipher, decipher; int tmp,in=0;
    cout << "Enter key (lowercase only): ";
    getline(cin, key);
    cout << "Enter Text (lowercase only): ";
    getline(cin, text);

    for (int i=0; i < text.length(); i++) {
        tmp = (text[i]-'a'+key[in]-'a')%26;
        cipher.push_back('A'+tmp);
        in = (in+1)%key.size();
    }
    
    cout << "Cipher: " << cipher << endl;
    in=0;
    
    for (int i=0; i < cipher.length(); i++) {
        tmp = (cipher[i]-'A' - (key[in]-'a')) % 26;
        if (tmp < 0)
            tmp += 26;
        decipher.push_back('a'+tmp);
        in = (in+1)%key.size();
    }

    cout << "Decipher: " << decipher << endl;
    return 0;
}