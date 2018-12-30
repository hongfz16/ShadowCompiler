extern int puts(char* c);
extern int putchar(char c);
extern int printf(char* c);
extern int scanf(char* c);

int fa(int b) {
	return b;
}

int main()
{
	int a;
	a = 99;
	int b;
	b = 1111;
	a = fa(b);
	printf("%d\n",a);
	return 0;
}