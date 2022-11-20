#include <iostream>
using namespace std;

int main() {

	int A[10] = { 30,29,28,27,26,25,24,23,22,21 }, B[10] = { 1,2,3,4,5,6,7,8,9,10 };
	for (int i = 0; i < 10; ++i) {
		int sum = A[i] + B[i];
		int difference = A[i] - B[i];
		A[i] = sum;
		B[i] = difference;
	}

}