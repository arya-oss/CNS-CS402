#include <iostream>
#include <stdint.h>
#include <stdio.h>
using namespace std;

uint8_t SB[256], T[256];

int main () {
    uint8_t key[] = { 0x08u, 0x17u, 0x26u, 0x35u, 0x44u, 0x53u, 0x62u, 0x71u };
    short keylen = 8;
    /* Initialization */
    for (int i=0; i<=255; i++) {
        SB[i] = i;
        T[i] = key[i%keylen];
    }
    /* Initial permutation of S */
    int j=0;
    for (int i=0; i <= 255; i++) {
        j = (j+SB[i]+T[i]) % 256;
        swap(SB[i], SB[j]);
    }
    
    char str [128]; uint8_t cipher[128];
    fgets (str, 128, stdin);
    
    int t,i=0; j=0; int len = 0;
    while ( str[len] != '\0') {
        i = (i+1) % 256;
        j = (j + SB[i]) % 256;
        swap(SB[i], SB[j]);
        t = (SB[i] + SB[j]) % 256;
        cipher[len] = str[len] ^ SB[t];
        len++;
    }
    printf("Cipher: ");
    for (i=0; str[i] != '\0'; i++) {
        printf("%x", cipher[i]);
    }

    /****************************
	 * Decryption Process		*
     ****************************/

    /* Initialization */
    for (i=0; i<=255; i++) {
        SB[i] = i;
        T[i] = key[i%keylen];
    }
    /* Initial permutation of S */
    j=0;
    for (int i=0; i <= 255; i++) {
        j = (j+SB[i]+T[i]) % 256;
        swap(SB[i], SB[j]);
    }
    
    char decipher[128];
    
    i=0; j=0; len = 0;
    while (str[len] != '\0') {
        i = (i+1) % 256;
        j = (j + SB[i]) % 256;
        swap(SB[i], SB[j]);
        t = (SB[i] + SB[j]) % 256;
        decipher[len] = cipher[len] ^ SB[t];
        len++;
    }

    printf("\nDecipher: ");
    for (i=0; str[i] != '\0'; i++) {
        printf("%c", decipher[i]);
    }

    return 0;
}
