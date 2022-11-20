#include <iostream>
using namespace std;

int getmax(int n[], int size) {
	int max = n[0];
	for (int i = 0; i < size; i++)
		if (n[i] > max)
			max = n[i];
}