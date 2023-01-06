#include <iostream>
using namespace std;

void swap(int A[], int B[], int size) {
	int temp;
	for (int i = 0; i < size; ++i) {
		temp = A[i];
		A[i] = B[i];
		B[i] = temp;
	}
}