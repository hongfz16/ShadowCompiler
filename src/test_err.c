int test5()
{
	int a;
	{
		int b;
		a = 1;
	}
	b = 2;
	printf("%d\n", a);
	return 0;
}