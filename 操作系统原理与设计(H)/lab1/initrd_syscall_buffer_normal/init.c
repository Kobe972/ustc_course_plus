#include<stdio.h>
#include<sys/syscall.h>
int main()
{
	char buf[15];
	int ret=syscall(548,buf,15);
	if(ret!=-1)
		printf("%s\n",buf);
	else
		printf("Bad buffer in initrd source code!\n");
	return 0;
}
