#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#define SHIFT 3

/**
 * Caesar Cipher
 * 
 * Author: Rajmani Arya
 * Date: 24 Aug 2016
 */

int main(int argc, char * argv[]) {
    
    string text, cipher, decipher; int tmp;
    cout << "Enter Text: ";
    
    getline(cin, text);

    for (int i=0; i < text.length(); i++) {
        if (islower(text[i]))
            cipher.push_back((text[i]-'a'+SHIFT)%26+ (int)'A');
        else
            cipher.push_back(text[i]);
    }
    cout << "Cipher: " << cipher << endl;

    for (int i=0; i < cipher.length(); i++) {
        if (isupper(cipher[i])) {
            tmp = cipher[i]-'A'-SHIFT;
            if (tmp < 0) {
                tmp += 26;
            }
            decipher.push_back(tmp+(int)'a');
        }
        else
            decipher.push_back(cipher[i]);
    }
    cout << "Decipher: " << decipher << endl;   
    return 0;
}