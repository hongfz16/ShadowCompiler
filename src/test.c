extern int printf(char* c);

int main() {
	int *b;
	int c;
	c = 999;
	b = &c;
	int a[10][6];
	a[0][0]=888;
	printf("%d %d\n", a[0][0], *b);
	return 0;
}