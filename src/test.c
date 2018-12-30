extern int printf(char* c);
extern int scanf(char* c);

int main() {
	int arr[10];
	int a;
	a = 1;
	a = a + 1;
	arr[a] = 1;
	arr[a=a+1] = 1;
	return 0;
}