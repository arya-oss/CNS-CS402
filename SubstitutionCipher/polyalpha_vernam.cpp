/**
 * Polyalphabatic Cipher
 * Vernam Cipher
 *
 * Author: Rajmani Arya
 * Date: 31st Aug 2016
 */


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char * argv[]) {
    
    string key, text, cipher, decipher;
    cout << "Enter key: ";
    getline(cin, key);
    cout << "Enter Text: ";
    getline(cin, text);
    if (key.length() != text.length() ){
        cout << "key and text length must be same \n";
        return 1;
    }

    for (int i=0; i < text.length(); i++) {
        cipher.push_back(key[i] ^ text[i]);
    }
    
    cout << "Cipher in Hex format\n";
    
    for (unsigned i = 0; i < cipher.length(); ++i) {
        printf("0x%x", cipher[i]);
    }

    for (int i=0; i < cipher.length(); i++) {
        decipher.push_back(key[i] ^ cipher[i]);
    }

    cout << "\nDecipher: " << decipher << endl;
    return 0;
}