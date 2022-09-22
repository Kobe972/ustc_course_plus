#include<stdio.h>
#include<windows.h>
int N,board[8][8];
int find(int n); //ур╣зnап
int main()
{
	scanf("%d",&N);
	find(0);
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++) printf("%d ",board[i][j]);
		printf("\n");
	}
	system("pause"); 
	return 0;
} 
int find(int n)
{
	if(n==N) return 1;
	int ok=1,complished=0,k,l;
	for(int i=0;i<N;i++)
	{
		board[i][n]=1;
		for(int j=0;j<n;j++)
		{
			if(board[i][j]==1) ok=0;
		}
		k=i-1;l=n-1;
		while(k>=0&&l>=0)
		{
			if(board[k][l]==1) ok=0;
			k--;
			l--;
		}
		k=i+1;l=n-1;
		while(k<N&&l>=0)
		{
			if(board[k][l]==1) ok=0;
			k++;
			l--;
		}
		if(ok&&find(n+1))
		{
			complished=1;
			break;
		}
		board[i][n]=0;
		ok=1;
	}
	if(complished) return 1;
	else return 0;
}
