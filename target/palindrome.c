extern int printf(char* c);
extern int scanf(char* c);
extern int strlen(char* s);

int main()
{
	char s[10086];
	scanf("%s", s);
	int len; 
	int mid;
	len = strlen(&s[0]);
	for(mid = len >> 1; mid >= 0; mid = mid - 1) 
	{
		if(s[mid] != s[len - 1 - mid])
		{
			printf("not palindrome!");
			return 0;
		}
	}
	printf("palindrome!");
	return 0;
}