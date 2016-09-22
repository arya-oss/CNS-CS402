/**
 * Onetime Pad cipher using Vernam Cipher
 * Vernam Cipher
 *
 * Author: Rajmani Arya
 * Date: 31st Aug 2016
 */


#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

void generateKey(string & s, unsigned int len) {
    for (unsigned i=0; i < len; i++) {
        s.push_back(rand()%256);
    }
}

int main(int argc, char * argv[]) {
    srand(time(NULL));
    string text, cipher, decipher, key;
    cout << "Enter Text: ";
    getline(cin, text);
    generateKey(key, text.length());

    cout << "\nKey in Hex format\n";
    
    for (unsigned i = 0; i < key.length(); ++i) {
        printf("%x", key[i]);
    }

    for (int i=0; i < text.length(); i++) {
        cipher.push_back(key[i] ^ text[i]);
    }
    
    cout << "\nCipher in Hex format\n";
    
    for (unsigned i = 0; i < cipher.length(); ++i) {
        printf("%x", cipher[i]);
    }

    for (int i=0; i < cipher.length(); i++) {
        decipher.push_back(key[i] ^ cipher[i]);
    }

    cout << "\nDecipher: " << decipher << endl;
    return 0;
}