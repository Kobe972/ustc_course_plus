#include<linux/string.h>
#include<linux/syscalls.h>
#include<linux/uaccess.h>
SYSCALL_DEFINE2(hello,char *,buf,size_t,buf_len)
{
	static const char s[]="Hello, world!\n";
	if(strlen(s)<=buf_len)
	{
		return copy_to_user(buf,s,sizeof(s));
	}
	else
	{
		return -1;
	}
}
