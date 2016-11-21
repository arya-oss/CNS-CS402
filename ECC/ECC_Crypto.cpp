#include <iostream>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

using namespace std;

/**
 * Elliptic Curve Cryptography
 * Author: Rajmani Arya
 * Date: 21 Nov 2016
 *
 */

struct Point {
public:
	mpz_t x, y;
	Point() {
		mpz_inits(x,y, NULL);
	}
	Point(mpz_t a, mpz_t b) {
		mpz_inits(x,y, NULL);
		mpz_set(x, a);
		mpz_set(y, b);
	}
	Point (int a, int b) {
		mpz_inits(x,y,NULL);
		mpz_set_si(x,a);
		mpz_set_si(y,b);
	}

	Point(const Point & p) {
		mpz_inits(x,y,NULL);
		mpz_set(x, p.x);
		mpz_set(y, p.y);
	}

	friend ostream & operator << (ostream & out, Point p) {
		gmp_printf("X=%Zd, Y=%Zd\n", p.x, p.y);
		return out;
	}
	/* Add a new point to this point and return a new point */
	Point addition (Point p, mpz_t prime) {
		Point r;
		mpz_t t,a,b,a1;
		mpz_inits(a, b, t, a1, NULL);
		// For X
		mpz_sub(a, y, p.y);
		mpz_sub(b, x, p.x);
		mpz_pow_ui(a, a, 2);
		mpz_pow_ui(b, b, 2);
		mpz_invert(a1, b, prime);
		mpz_mul(t, a, a1);
		mpz_sub(t, t, x);
		mpz_sub(t, t, p.x);
		mpz_mod(r.x, t, prime);
		// For Y
		mpz_sub(b, p.x, x);
		mpz_sub(a, p.y, y);
		mpz_invert(a1, b, prime);
		mpz_mul(t, a, a1);
		mpz_sub(a1, x, r.x);
		mpz_mul(t, t, a1);
		mpz_sub(t, t, y);
		mpz_mod(r.y, t, prime);
		mpz_clears(a,b,t,a1, NULL);
		return r;
	}
	/* Double this point and return new resultant point */
	Point Double(mpz_t coff, mpz_t prime) {
		Point r;
		mpz_t t,a,b,a1,b1;
		mpz_inits(a, b, t, a1, b1, NULL);

		mpz_pow_ui(a, x, 2);
		mpz_mul_ui(a, a, 3);
		mpz_add(a, a, coff);

		mpz_mul_ui(b, y, 2);
		mpz_invert(a1, b, prime);
		// T is having slope
		mpz_mul(t, a, a1);
		mpz_pow_ui(a, t, 2);
		mpz_mul_ui(b, x, 2);
		mpz_sub(a, a, b);
		mpz_mod(r.x, a, prime);
		mpz_sub(a, x, r.x);
		mpz_mul(a, a, t);
		mpz_sub(a, a, y);
		mpz_mod(r.y, a, prime);
		mpz_clears(t,a,b,a1, NULL);
		return r;
	}
	/* Perform scaler multiplication and results based on add and double */
	Point ScalerMul(mpz_t number, mpz_t coff, mpz_t prime) {
    	Point r,r1;
    	mpz_set(r.x, x);
    	mpz_set(r.y, y);
    	Point res;
    	bool flag = false;
    	mpz_t a,b,c,q,scaler;
    	mpz_inits(a,b,c,q,scaler,NULL);
    	mpz_set(scaler, number);
		// LSB Multiplication
    	while(mpz_cmp_ui(scaler, 0) != 0)
    	{
    		mpz_fdiv_q_ui(q, scaler, 2);
    		mpz_mod_ui(a, scaler, 2);
    		mpz_fdiv_q_ui(scaler, scaler, 2);
    		if (mpz_cmp_ui(a, 1) == 0) {
    			if (!flag) {
    				flag = true;
    				res = r;
		    		r = r.Double(coff, prime);
    				continue;
    			}
    			r1 = r.addition(res, prime);
    			mpz_set(res.x, r1.x);
    			mpz_set(res.y, r1.y);
    		}
    		r = r.Double(coff, prime);
    	}
    	return res;
	}
};

int main () {
	/* curve: y^2 = x^2 + 4x + 20 */
	mpz_t a,b,n,prime;
	mpz_inits (a,n,b,prime, (mpz_ptr)NULL);
	mpz_set_ui(prime, 29);
	mpz_set_ui(a, 4);
	mpz_set_ui(b, 20);
	mpz_set_ui(n, 5);
	Point G;
	mpz_set_ui(G.x, 1);
	mpz_set_ui(G.y, 5);

	// Point p = G.ScalerMul(n,a,prime);
	// cout << p;

	unsigned long seed;
    seed = time(NULL);

    gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);
    gmp_randseed_ui(rstate, seed);

    // ECDSA
    mpz_t na,nb,k;
    mpz_inits(na,k,nb, NULL);
    mpz_urandomm(na, rstate, n);
    while (mpz_cmp_ui(na, 0) == 0) {
    	mpz_urandomm(na, rstate, n);
    }
    gmp_printf("\nA's private key: %Zd\n", na);
    // A's Public Key
    Point pub_key_A = G.ScalerMul(na, a, prime);
    cout << "A's public key: " << pub_key_A;
    mpz_urandomm(nb, rstate, n);
    while (mpz_cmp_ui(nb, 0) == 0) {
    	mpz_urandomm(nb, rstate, n);
    }
    gmp_printf("\nB's private key: %Zd\n", nb);

    // B's Public Key
    Point pub_key_B = G.ScalerMul(nb, a, prime);
    cout << "B's public key: " << pub_key_B;

    Point text; // Plain Text (15, 2)
    mpz_set_ui(text.x, 15);
    mpz_set_ui(text.y, 2);
    cout << "Plain Text: " << text;
    mpz_urandomm(k, rstate, n);
    while (mpz_cmp_ui(k, 0) == 0) {
    	mpz_urandomm(k, rstate, n);
    }
    Point c1 = G.ScalerMul(k, a, prime);
    Point c2 = pub_key_B.ScalerMul(k, a, prime);
    c2 = c2.addition(text, prime);

    cout << "Cipher: " << c1 << c2 << "cipher ends\n";

    Point d1 = c1.ScalerMul(nb, a, prime);
    mpz_sub(d1.y, d1.y, -1);
    // mpz_mod(d1.y, d1.y, prime);
    Point d = c2.addition(d1, prime);
    cout <<"Decipher: " << d;

	return 0;
}
