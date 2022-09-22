#include<stdio.h>
#define SWAP(x,y) {int len=sizeof(x);char tmp;while(len-->0){tmp=*((char*)&x+len);*((char*)&x+len)=*((char*)&y+len);*((char*)&y+len)=tmp;}}
int main()
{
	int x,y;
	scanf("%d%d",&x,&y);
	SWAP(x,y);
	printf("result:x=%d,y=%d\n",x,y);
	while(1);
	return 0;
}
