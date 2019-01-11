extern int printf(char* c);
extern int scanf(char* c);
extern int strlen(char* s);

int main() {
	char rS[1024];
	char rT[1024];
	char* S;
	char* T;
	int nxt[1024];
	int lenS;
	int lenT;
	int i;
	int j;
	int flag;
	flag = 0;
	T = &rT[0]; 
	S = &rS[0];

	scanf("%s%s",S, T);
	lenS = strlen(S);
	lenT = strlen(T);

	nxt[0] = -1;
	j = -1;
	for (i = 1; i < lenT;  i = i + 1) {
		for (j; j >= 0 && T[i] != T[j+1]; j = nxt[j]);
		if (T[i] == T[j+1]) {
            j = j+1;
        }
		nxt[i] = j;
	}
	i = 0;
	for (j = -1; i < lenS; i = i + 1) {
		for (j; j >= 0 && S[i] != T[j+1]; j = nxt[j]);
		if (S[i] == T[j+1]) {
            j = j + 1;
        }
		if (j == lenT-1) {
			printf("%d\n", i - j);
			flag = 1;
			j = nxt[j];
		}
	}
	if (flag == 0){
		printf("False\n");
    }

	return 0;
}
