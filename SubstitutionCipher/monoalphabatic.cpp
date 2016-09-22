#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <time.h>

using namespace std;

/**
 * Monoalphabatic Cipher
 * 
 * Author: Rajmani Arya
 * Date: 24 Aug 2016
 */

int main(int argc, char * argv[]) {
    srand(time(NULL));
    string text, cipher, decipher; int tmp;
    cout << "Enter Text: ";
    
    vector<int> keys(26);
    vector<int> r_keys(26);
    
    for (int i=0; i < keys.size(); i++) {
        keys[i] = i;
    }

    random_shuffle (keys.begin(), keys.end());

    for (int i=0; i < keys.size(); i++) {
        r_keys[keys[i]] = i;
    }

    getline(cin, text);

    for (int i=0; i < text.length(); i++) {
        if (islower(text[i]))
            cipher.push_back(keys[text[i]-'a'] + (int)'A');
        else
            cipher.push_back(text[i]);
    }
    cout << "Cipher: " << cipher << endl;

    for (int i=0; i < cipher.length(); i++) {
        if (isupper(cipher[i])) {
            decipher.push_back(r_keys[cipher[i]-'A']+(int)'a');
        }
        else
            decipher.push_back(cipher[i]);
    }
    cout << "Decipher: " << decipher << endl;   
    return 0;
}