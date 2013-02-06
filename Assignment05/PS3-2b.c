////////////////////////////////////////////
//     Brian Arnberg - ELEC6260           //
////////////////////////////////////////////
//     Problem Set 3, Problem 2           //
//       Execute the C program            //
//                                        //
//                                        //
////////////////////////////////////////////
#include <stdio.h>
#include <stm32f4xx.h>

int k;	//global variable

int f1(int x1, int x2) {
	return x1 + x2;
}

int f2(int x1) {
	return x1 + 1; 
}

void f3(int r) {
	int j;
	for (j = 0; j < 2; j++)
		k = k + f1(r + j, 5);
}

void main() {
	int a;
	k = 0;
	f3(3);
	a = f2(2);
}
