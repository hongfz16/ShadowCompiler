extern int printf(char* c);

int main() {
	printf("before");
	if(1){
		printf("right");
	}
	printf("out");
	return 0;
}