extern int puts(char* c);
extern int getchar();
extern int putchar(char c);
extern int printf(char* c);
extern int scanf(char* c);

int appendChar(char* s, int len, char c)
{
	*(s+len) = c;
	len = len + 1;
	s[len] = '\0';
	return len;
}

int get_str(char* str)
{
	int len;
	len = -1;
	char c;
	for(c = getchar(); c != '\n'; c = getchar())
		str[len = len + 1] = c;
	str[len = len + 1] = '\0';
	return len;
}

int getResult(char* exp, int len)
{
	int result;
	result=0;
	len = appendChar(exp, len, '#');
	char myStack[1000];
	char* pmyStack;
	pmyStack = &myStack[0];
	int top;
	top = 0;
	myStack[top = top + 1] = '#';
	char xxreversePolishExp[1086];
	char* reversePolishExp;
	reversePolishExp = &xxreversePolishExp[0];
	int reverseLen;
	reverseLen = 0;
	int i;
	i=0;
	while(top)
	{
		while(exp[i]<='9' && exp[i]>='0')
		{
			reverseLen = appendChar(reversePolishExp, reverseLen, exp[i]);
			i = i + 1;
		}
		reverseLen = appendChar(reversePolishExp, reverseLen, '|');
		char temp;
		temp=exp[i];
		if(temp=='#')
		{
			char topTemp;
			topTemp = myStack[top];
			while(topTemp!='#')
			{
				top = top - 1;
				reverseLen = appendChar(reversePolishExp, reverseLen, topTemp);
				topTemp = myStack[top];
			}
			top = top - 1;
		}
		else if(temp=='*' || temp=='/')
		{
			char topTemp;
			topTemp = myStack[top];
			while(topTemp=='*' || topTemp=='/')
			{
				top = top - 1;
				reverseLen = appendChar(reversePolishExp, reverseLen, topTemp);
				topTemp = myStack[top];
			}
			myStack[top = top + 1] = temp;
			i = i + 1;
		}
		else if(temp=='+' || temp=='-')
		{
			char topTemp;
			topTemp = myStack[top];
			while(topTemp!='#')
			{
				top = top - 1;
				reverseLen = appendChar(reversePolishExp, reverseLen, topTemp);
				topTemp = myStack[top];
			}
			myStack[top = top + 1] = temp;
			i = i + 1;
		}
	}
	
	reverseLen = appendChar(reversePolishExp, reverseLen, '#');
	int calculateStack[10086];
	int ctop;
	ctop = 0;
	for(i=0;i<reverseLen;i = i+1)
	{
		int flag;
		flag = 0;
		int num;
		num = 0;
		while(reversePolishExp[i]<='9' && reversePolishExp[i]>='0')
		{
			flag = 1;
			int temp;
			temp = reversePolishExp[i];
			num=num*10+(temp-48);
			i = i + 1;
		}
		if(flag)
			calculateStack[ctop = ctop + 1] = num;
		char temp;
		temp=reversePolishExp[i];
		if(temp=='|')
			continue;
		if(temp=='#')
		{
			if(ctop > 1)
			{
				puts("ERROR not empty");
				return -1;
			}
		}
		else
		{
			if(ctop < 2)
			{
				puts("ERROR less than 2 elements in stack");
				return -1;
			}
			int first ;
			first = calculateStack[ctop];
			ctop = ctop - 1;
			int second;
			second = calculateStack[ctop];
			ctop = ctop - 1;
			if(temp == '+')
				calculateStack[ctop = ctop + 1] = first + second;
			else if(temp == '-')
				calculateStack[ctop = ctop + 1] = second - first;
			else if(temp == '*')
				calculateStack[ctop = ctop + 1] = second * first;
			else if(temp == '/')
				calculateStack[ctop = ctop + 1] = second / first;
			else
				calculateStack[ctop = ctop + 1] = 0;

		}
	}
	result = calculateStack[ctop];
	return result;
}



int main()
{
	char targetExp[10034];
	int len;
	scanf("%s", targetExp);
	for(len=0;targetExp[len]!='\0';len=len+1){
		len = len;
	}
	int result;
	result = getResult(&targetExp[0], len);
	printf("%d\n",result);
	return 0;
}