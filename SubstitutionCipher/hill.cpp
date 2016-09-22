#include <bits/stdc++.h>
using namespace std;
#define N 3

/**
 * Title - Hill Cipher
 *
 * Author: Rajmani Arya
 * Date: 31st Aug 2016
 *
 * @see Matrix Inverse > www.geeksforgeeks.org/adjoint-inverse-matrix/
 * @see Extended Euclidean > www.geeksforgeeks.org/basic-and-extended-euclidean-algorithms/
 */

void getCofactor(int A[N][N], int temp[N][N], int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = A[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int determinant(int A[N][N], int n) {
    int D = 0; 
    if (n == 1)
        return A[0][0];
    int temp[N][N];
    int sign = 1;
    for (int f = 0; f < n; f++) {
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return D;
}

void adjoint(int A[N][N],int adj[N][N]) {
    if (N == 1) {
        adj[0][0] = 1;
        return;
    }
    int sign = 1, temp[N][N];
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            getCofactor(A, temp, i, j, N);
            sign = ((i+j)%2==0)? 1: -1;
            adj[j][i] = (sign)*(determinant(temp, N-1));
        }
    }
}

int gcdExtended(int a, int b, int *x, int *y)
{
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
 
    int x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1; 
    return gcd;
}

bool inverse(int A[N][N], int inverse[N][N]) {
    int det = determinant(A, N);
    if (det == 0) {
        cout << "Singular matrix, can't find its inverse";
        return false;
    }

    det %= 26;
    if (det < 0) {
        det += 26;
    }

    int x,y;

    gcdExtended(det, 26, &x, &y);
    if (x < 0) {
        x += 26;
    }

    int adj[N][N];
    adjoint(A, adj);

    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++) {
            inverse[i][j] = (adj[i][j] * x)%26;
            if (inverse[i][j] < 0)
                inverse[i][j] += 26;
        }
 
    return true;
}

int main() {
    int key[N][N] = {
        {17, 17, 5},
        {21, 18, 21},
        {2, 2, 19}
    };
    int r_key[N][N]; int col_arg[N][1], col_res[N][1];
    if (inverse(key, r_key) == false) {
        cout << "Not good key ! try different !" << endl;
        return 1;
    }
    // for (int i = 0; i < N; ++i) {
    //     for (int j=0; j<N; ++j) {
    //         cout << r_key[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    string text, cipher, decipher;
    cout << "Enter Text: "; 
    getline(cin, text);
    int pad_len = text.length()%N;
    /* 'x' is padded for making length multiple of N */
    while(pad_len -- ) {
        text.push_back('x');
    }
    for (int i=0; i < text.length(); i+=N) {
        if (!islower(text[i])) {
            cout << "Text must be lowercase only !" << endl;
            return 1;
        }
        for (int j=0; j < N; j++) {
            col_arg[j][0] = text[i+j]-'a';
        }
        for (int p=0; p < N; p++) {
            col_res[p][0] = 0;
            for (int q=0; q < N; q++) {
                col_res[p][0] += key[q][p] * col_arg[q][0];
            }
            // cout << col_res[p][0] << endl;
            col_res[p][0] %= 26;
        }
        for (int j=0; j<N; j++) {
            cipher.push_back('A'+col_res[j][0]);
        }
    }

    cout << "Cipher: " << cipher << "\n";

    for (int i=0; i < cipher.length(); i+=N) {
        if (!isupper(cipher[i])) {
            cout << "Cipher must be uppercase only !" << i << endl;
            return 1;
        }
        for (int j=0; j < N; j++) {
            col_arg[j][0] = cipher[i+j]-'A';
        }
        for (int p=0; p < N; p++) {
            col_res[p][0] = 0;
            for (int q=0; q < N; q++) {
                col_res[p][0] += r_key[q][p] * col_arg[q][0];
            }
            // cout << col_res[p][0] << endl;
            col_res[p][0] %= 26;
        }
        for (int j=0; j<N; j++) {
            decipher.push_back('a'+col_res[j][0]);
        }
    }

    cout << "Decipher: " << decipher << "\n";
    return 0;
}