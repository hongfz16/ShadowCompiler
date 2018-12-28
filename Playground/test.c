#include<stdio.h>
int main() {
	int a[10][10][10];
	int b[10][10];
	int (*pa)[10];
	int x[10];
	// pa  = a[1]; 
	// int *((*apa)[10][10]);//= {a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]};
	int *(ap[10]);
	// int xx = 10;
	// printf("%d, %d\n", ap, ap+1);
	// ap[0] = &xx;
	// apa[0] = pa;
	a[1][2][3] = 12;
	x[2] = 777;
}
