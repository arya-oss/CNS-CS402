#include <iostream>
#include <string>

using namespace std;

int main () {
	string num1, num2, res, len1;
	cout << "Enter Number: "; cin >> num1;
	cout << "Enter Number: "; cin >> num2;
	int prime;
	cout << "Enter prime number: ";
	cin >> prime;
	int carry=0, sum;
	int i = num1.length()-1;
	int j = num2.length()-1;
	/* sum from last digits untill one of numbers is reached to less than zero index */
	for (; i >= 0 && j>=0 ; i--, j--) {
		sum = (carry+(num1[i]-'0')+(num2[j]-'0'));
		carry = sum/10;
		sum = sum%10;
		res.insert(0, 1, sum+'0');
	}
	/* add number which is left */
	while(i>=0) {
		sum = (carry+num1[i]-'0');
		carry = sum/10;
		sum = sum%10;
		res.insert(0, 1, sum+'0');
		i--;
	}
	while(j>=0) {
		sum = (carry+num2[j]-'0');
		carry = sum/10;
		sum = sum%10;
		res.insert(0, 1, sum+'0');
		j--;
	}
	/* if carry left then add it to result number */
	if (carry)
		res.insert(0, 1, carry+'0');
	/**
	 * Modular Arithmetic for finding modulo
	 * remainder = 0
	 * for each digit from begining
	 * remainder = (10 * remainder + current_digit) % num;
	 */
	carry = 0;
	for (i=0; i < res.length(); i++) {
		carry = (10*carry + (res[i]-'0'))%prime;
	}
	
	cout << "(";
	cout << num1 << "+" << num2 << ") % " << prime << " = " << carry << "\n";
	return 0;
}