
#include <iostream>
using namespace std;

double calculateScore(int yardSize,int acornCount, int pileCount,int opperationCount){
	int numerator = 2 * acornCount * yardSize * yardSize* yardSize;
	int denominator = 3 * pileCount;
	double result = (double)numerator / denominator - opperationCount;
	return result;
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
