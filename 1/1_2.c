#include <stdio.h>

int func(int n);
int main ()
{
	int n;
	scanf("%d",&n);
	printf("%d",func(n));
	printf("\n");
	return 0;
}

int func (int n)
{
	if(n==1){
	return 1;
	}
	else {
	 return n*func(n-1); //b
	}
}
