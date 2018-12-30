extern int puts(char* c);
extern int printf(char* c);

int haha() {
	printf("%d", 3);
	return 0;
}

int main()
{
	int a;
	int b;
	printf("%d", 1);
	{
		int a;
		printf("%d", 2);
	}
	int b;
	haha();
	return 0;
}