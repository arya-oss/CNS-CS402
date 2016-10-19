#include <iostream>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

using namespace std;

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
	/* Perform scaler multiplication and results based on add and doublr */
	Point ScalerMul(int scaler, mpz_t coff, mpz_t prime) {
    	Point r,r1;
    	mpz_set(r.x, x);
    	mpz_set(r.y, y);
    	Point res;
    	bool flag = false;
    	if (scaler & 1) {
    		mpz_set(res.x, r.x);
	    	mpz_set(res.y, r.y);
    		flag = true;
    	}
		// LSB Multiplication
    	for (int i=1; i<8; i++) {
    		r = r.Double(coff, prime);
    		if (scaler & (1 << i)) {
    			if (!flag) {
    				flag = true;
    				res = r;
    				continue;
    			}
    			r1 = r.addition (res, prime);
    			mpz_set(res.x, r1.x);
    			mpz_set(res.y, r1.y);
    		}
    	}
    	return res;
	}
};

int main () {
	/* curve: y^2 = x^2 + 4x + 20 */
	mpz_t a,b,prime;
	mpz_inits (a, b, prime, (mpz_ptr)NULL);
	mpz_set_ui(prime, 29);
	mpz_set_ui(a, 4);
	mpz_set_ui(b, 20);

	// Point p1, p2;
	// mpz_set_ui(p1.x, 5);
	// mpz_set_ui(p1.y, 22);
	// mpz_set_ui(p2.x, 16);
	// mpz_set_ui(p2.y, 27);
	// Point p3 = p1.addition(p2, prime);
	// cout << p1 << p2;
	// Point p3 = p1.Double(a, prime);
	// cout << p3;
	Point p1,p2;
	mpz_set_ui(p1.x, 1);
	mpz_set_ui(p1.y, 5);

	for (int i=1; i <=36; i++) {
		p2 = p1.ScalerMul(i, a, prime);
		cout << p2;
	}
	return 0;
}
