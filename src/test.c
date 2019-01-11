extern int printf(char* c);
extern int scanf(char* c);


int combine3(int* vec, int n, int* dest)
{
	int i;
	for(i = 0; i < n ; i = i + 1)
	{
		*dest = *dest + vec[i];
	}
	printf("combine3 gets: %d\n", *dest);
	return 0;
}

int test1() {
	int a[5];
	int* p;
	int dest;
	int i;
	for(i = 0; i < 5; i = i + 1)
	{
		a[i] = i;
	}
	p = &dest;
	combine3(&a[1], 4, p);
	combine3(&a[2], 3, &a[3]);
	return 0;
}

int test3()
{
	int (*a)[10];
	int b;
	a[2] = &b;
	*(a[2]) = 3;
	printf("test3 gets: %d\n", b);
	return 0;
}

int test4()
{
	char *(a[16]);
	printf("test4 gets %p %p\n", (a), a+1);
	return 0;
}

int print_int(int d)
{
	char a[40];
	int top;
	for(top = 0; d; d = d / 10)
		a[top = top + 1] = d % 10 + 48;
	while(top)
	{
		printf("%c", a[top]);
		top = top - 1;
	}
	return 0;
}

int test6()
{
	int i;
	for(i = 10; i; i = i - 1)
	{
		if(i == 5) continue;
		if(i == 3) break;
		printf("%d ", i);
	}
	return 0;
}

int test2()
{
	int arr[10];
	int a;
	a = 1;
	a = a + 1;
	arr[a] = 1;
	arr[a=a+1] = 1;
	return 0;
}

int main() {
	test6();
	return 0;
}

