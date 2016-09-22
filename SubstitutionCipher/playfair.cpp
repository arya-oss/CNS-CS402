
/**
 * Title: Playfair Cipher
 * 
 * Author: Rajmani Arya
 * Date: 31 Aug 2016
 * 
 * 'j' character is removed from text and box
 */
 
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>

using namespace std;

const string key="monarchy";

char poly[5][5];

map<char, pair<int, int> > look;

void fill_poly() {
    int arr[26];
    for (int i=0; i<26; i++)
        arr[i]=0;
    
    int in = 0;
    for (int i=0; i<key.length(); i++) {
        if (arr[key[i]-'a'] == 0) {
            if (key[i] == 'i' || key[i] == 'j') {
                poly[in/5][in%5] = 'I';
                arr['i'-'a'] = arr['j'-'a'] = 1;
            } else {
                poly[in/5][in%5] = key[i]-32;
                arr[key[i]-'a'] = 1;
            }
            in++;
        }
    }

    for (int i=0; i<26; i++) {
        if (arr[i] == 0) {
            if (i == 8){ // i == 'I'
                poly[in/5][in%5] = 'I';
                arr[i] = 1;
                arr[++i] = 1;
            } else {
                poly[in/5][in%5] = 'A'+i;
            }
            in++;
        } 
    }
    // creating map of char to co-ordinates
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++){
            look.insert(make_pair(poly[i][j]+32, make_pair(i,j)));
        }
    }
}

string crypt(string s) {
    if (s[0] == 'j') s[0] = 'j';
    if (s[1] == 'j') s[1] = 'j';
    pair<int, int> p1 = look[s[0]];
    pair<int, int> p2 = look[s[1]];
    string c="00";

    if (p1.first == p2.first) { // same row
        c[0] = poly[p1.first][(p1.second+1)%5];
        c[1] = poly[p2.first][(p2.second+1)%5];
    } else if (p1.second == p2.second) { // same col
        c[0] = poly[(p1.first+1)%5][p1.second];
        c[1] = poly[(p2.first+1)%5][p2.second];
    } else { // else anywhere on board
        c[0] = poly[p1.first][p2.second];
        c[1] = poly[p2.first][p1.second];
    }
    return c;
}

string decrypt(string s) {
    pair<int, int> p1 = look[s[0]+32]; // s is uppercase and lookup is for lowercase
    pair<int, int> p2 = look[s[1]+32]; // so add 32 to convert
    string c="00";

    if (p1.first == p2.first) { // same row
        c[0] = poly[p1.first][p1.second==0 ? 5 : p1.second-1] + 32;
        c[1] = poly[p2.first][p2.second==0 ? 5 : p2.second-1] + 32;
    } else if (p1.second == p2.second) { // same col
        c[0] = poly[p1.first==0?5:p1.first-1][p1.second] + 32;
        c[1] = poly[p2.first==0?5:p2.first-1][p2.second] + 32;
    } else { // else anywhere on board
        c[0] = poly[p1.first][p2.second] + 32;
        c[1] = poly[p2.first][p1.second] + 32;
    }
    return c;
}

int main(int argc, char * argv[]) {
    string text, cipher="", decipher; int tmp;
    fill_poly();
    // for(unsigned i = 0; i < 5; ++i) {
    //     for(unsigned j = 0; j < 5; ++j) {
    //         cout << poly[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    cout << "Enter Text: ";
    getline(cin, text);

    for (int i=0; i < text.length(); i+=2) {
        if (text[i] == text[i+1]) {
            string s;
            s.push_back(text[i]);
            s.push_back((rand()%26)+'a');
            cipher += crypt(s);
            i--;
        } else {
            string s;
            s.push_back(text[i]);
            s.push_back(text[i+1]);
            cipher += crypt(s);
        }
    }

    cout << "Cipher: " << cipher << endl;

    for (int i=0; i < cipher.length(); i+=2) {
        string s;
        s.push_back(cipher[i]);
        s.push_back(cipher[i+1]);
        decipher += decrypt(s);
    }

    cout << "Decipher: " << decipher << endl;   
    return 0;
}