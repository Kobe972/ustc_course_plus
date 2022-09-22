#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
int main(int argc,char *argv[])
{
	pid_t pid=fork();
	int status;
	switch(pid)
	{
		case -1:
			exit(1);
		case 0:
			ptrace(PTRACE_TRACEME,0,0,0);
			execvp(argv[1],argv+1);
			exit(1);
	}
	waitpid(pid,0,0);
	ptrace(PTRACE_SETOPTIONS,pid,0,PTRACE_O_EXITKILL);
	while(1)
	{
		ptrace(PTRACE_SYSCALL,pid,0,0);
		waitpid(pid,0,0);
		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS,pid,0,&regs);
		long syscall=regs.orig_rax;
		fprintf(stderr,"%ld(%ld, %ld, %ld, %ld, %ld, %ld)",syscall,(long)regs.rdi,(long)regs.rsi,(long)regs.rdx,(long)regs.r10,(long)regs.r8,(long)regs.r9);
		ptrace(PTRACE_SYSCALL,pid,0,0);
		waitpid(pid,&status,0);
		ptrace(PTRACE_GETREGS, pid, 0, &regs);
		fprintf(stderr, " = %ld\n", (long)regs.rax);
		if(WIFEXITED(status)) break;
	}
	return 0;
}

