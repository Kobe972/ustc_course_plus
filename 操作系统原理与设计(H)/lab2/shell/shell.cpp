// IO
#include <iostream>
// std::string
#include <string>
// std::vector
#include <vector>
// std::string 转 int
#include <sstream>
// PATH_MAX 等常量
#include <climits>
// POSIX API
#include <unistd.h>
// wait
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fstream>
#include <linux/input.h>

std::vector<std::string> history;
std::vector<std::string> split(std::string s, const std::string &delimiter);
void trim(std::string &s);
int command_pipe(std::string cmd,int in_fd=STDIN_FILENO,int out_fd=STDOUT_FILENO);
std::string find_arg(std::string &s,const std::string &arg_mark);
static void ctrlc(int sig)
{
  fflush(stdin);
  std::cout<<std::endl;
  return;
}
static void ctrlz(int sig)
{
  FILE *bash_history;
  bash_history=fopen(".bash_history","w");
  for(int i=0;i<history.size();i++)
  {
    fprintf(bash_history,"%s\n",history[i].c_str());
  }
  fclose(bash_history);
  raise(SIGSTOP);
}
static void quit(int sig)
{
  exit(0);
}
int main() {
  // 不同步 iostream 和 cstdio 的 buffer
  std::ios::sync_with_stdio(false);

  // 用来存储读入的一行命令
  std::string cmd;
  char _cmd[1000];
  signal(SIGINT,ctrlc);
  signal(SIGTSTP,ctrlz);
  FILE *bash_history;
  std::ifstream inFile(".bash_history", std::ios::in);
  if(inFile)
  {
    std::string hist_cmd;
    while(std::getline(inFile,hist_cmd))
    {
      history.push_back(hist_cmd);
    }
  }
  while (true) {
    // 打印提示符
    std::cout << "# "<<std::flush;
    //readline with subprocess
    pid_t input_pid,ppid=getpid(),ctrld_pid;
    int input_pipe[2];
    if(pipe(input_pipe)==-1)
    {
      std::cout<<"Pipe error\n";
      return -1;
    }
    ctrld_pid=fork();
    if(ctrld_pid==0)
    {
      while(1)
      {
        int ctrl_pressed=0;
        struct input_event ev;
        FILE *kbd = fopen("/dev/input/by-id/usb-ITE_Tech._Inc._ITE_Device_8910_-event-kbd", "r");
        while (fread(&ev, sizeof(ev), 1, kbd) == 1)
        {
          if (ev.type == EV_KEY&&ev.code==29)
          {
            if(ev.value==1) ctrl_pressed=1;
            if(ev.value==0) ctrl_pressed=0;
          }
          if(ev.type==EV_KEY&&ev.code==32)
          {
            if(ctrl_pressed)
            {
              fclose(kbd);
              kill(ppid,SIGTSTP);
              return 0;
            }
          }
          // similarly for KEY_LEFTCTRL, KEY_RIGHTCTRL, etc.
        }
      }
    }
    input_pid=fork();
    strcpy(_cmd,"");
    if(input_pid==0)
    {
      signal(SIGINT,quit);
      std::string _cmd;
      std::getline(std::cin,_cmd);
      close(input_pipe[0]);
      write(input_pipe[1],_cmd.c_str(),_cmd.length()+1);
      close(input_pipe[1]);
      exit(-1);
    }
    wait(nullptr);
    close(input_pipe[1]);
    read(input_pipe[0],_cmd,1000);
    close(input_pipe[0]);
    cmd=std::string(_cmd);
    if(cmd=="") continue;
    history.push_back(cmd);
    trim(cmd);
    if(cmd=="") history.pop_back(); //Exclude blank commands
    // 按空格分割命令为单词
    std::vector<std::string> args = split(cmd, " ");

    // 没有可处理的命令
    if (args.empty()) {
      continue;
    }
    //Implement history commands
    if(args[0][0]=='!')
    {
      if(args[0].length()<=1)
      {
        continue; //No need to pop
      }
      if(args[0][1]!='!')
      {
        int index=atoi(args[0].c_str()+1);
        if(index==0||index>history.size())
        {
          std::cout<<"bash: "<<args[0]<<": event not found\n";
          continue;
        }
        history.pop_back();
        history.push_back(history[index-1]);
        cmd=history[index];
      }
      else
      {
        history.pop_back();
        cmd=history[history.size()-1];
      }
      args = split(cmd, " ");
      // 没有可处理的命令
      if (args.empty()) {
        continue;
      }
      std::cout<<cmd<<std::endl; //Flush in order to keep the output order.
    }
    //Search for history
    if(args[0]=="history")
    {
      if(args.size()<=1)
      {
        std::cout<<"Insufficient arguments\n";
        continue;
      }
      for(int i=atoi(args[1].c_str())<=history.size()?atoi(args[1].c_str()):history.size();i>=1;i--)
      {
        std::cout<<history.size()-i+1<<"\t"<<history[history.size()-i]<<"\n";
      }
      continue;
    }
    if(args[0]=="echo")
    {
      if (args.size() <= 1) {
        // 输出的信息尽量为英文，非英文输出（其实是非 ASCII 输出）在没有特别配置的情况下（特别是 Windows 下）会乱码
        // 如感兴趣可以自行搜索 GBK Unicode UTF-8 Codepage UTF-16 等进行学习
        std::cout << "Insufficient arguments\n";
        // 不要用 std::endl，std::endl = "\n" + fflush(stdout)
        continue;
      }
      if(args[1]=="~root")
      {
        std::cout<<"/root\n";
        continue;
      }
      else if(args[1]=="$SHELL")
      {
        char self_path[1024];
        readlink("/proc/self/exe",self_path,sizeof(self_path));
        std::cout<<self_path<<"\n";
        continue;
      }
    }
    // 更改工作目录为目标目录
    if (args[0] == "cd") {
      if (args.size() <= 1) {
        // 输出的信息尽量为英文，非英文输出（其实是非 ASCII 输出）在没有特别配置的情况下（特别是 Windows 下）会乱码
        // 如感兴趣可以自行搜索 GBK Unicode UTF-8 Codepage UTF-16 等进行学习
        std::cout << "Insufficient arguments\n";
        // 不要用 std::endl，std::endl = "\n" + fflush(stdout)
        continue;
      }

      // 调用系统 API
      int ret = chdir(args[1].c_str());
      if (ret < 0) {
        std::cout << "cd failed\n";
      }
      continue;
    }

    // 显示当前工作目录
    if (args[0] == "pwd") {
      std::string cwd;

      // 预先分配好空间
      cwd.resize(PATH_MAX);

      // std::string to char *: &s[0]（C++17 以上可以用 s.data()）
      // std::string 保证其内存是连续的
      const char *ret = getcwd(&cwd[0], PATH_MAX);
      if (ret == nullptr) {
        std::cout << "cwd failed\n";
      } else {
        std::cout << ret << "\n";
      }
      continue;
    }

    // 设置环境变量
    if (args[0] == "export") {
      for (auto i = ++args.begin(); i != args.end(); i++) {
        std::string key = *i;

        // std::string 默认为空
        std::string value;

        // std::string::npos = std::string end
        // std::string 不是 nullptr 结尾的，但确实会有一个结尾字符 npos
        size_t pos;
        if ((pos = i->find('=')) != std::string::npos) {
          key = i->substr(0, pos);
          value = i->substr(pos + 1);
        }

        int ret = setenv(key.c_str(), value.c_str(), 1);
        if (ret < 0) {
          std::cout << "export failed\n";
        }
      }
    }

    // 退出
    if (args[0] == "exit") {
      if (args.size() <= 1) {
        return 0;
      }

      // std::string 转 int
      std::stringstream code_stream(args[1]);
      int code = 0;
      code_stream >> code;

      // 转换失败
      if (!code_stream.eof() || code_stream.fail()) {
        std::cout << "Invalid exit code\n";
        continue;
      }

      return code;
    }
    std::string in_redir=find_arg(cmd,"<"),out_redir_append=find_arg(cmd,">>"),out_redir_create=find_arg(cmd,">");
    if(in_redir!="") freopen(in_redir.c_str(),"r",stdin);
    if(out_redir_create!="") freopen(out_redir_create.c_str(),"w",stdout);
    else if(out_redir_append!="") freopen(out_redir_append.c_str(),"a",stdout);
    command_pipe(cmd);
    freopen("/dev/tty","r",stdin);
    freopen("/dev/tty","w",stdout);
  }
}

// 经典的 cpp string split 实现
// https://stackoverflow.com/a/14266139/11691878
std::vector<std::string> split(std::string s, const std::string &delimiter) {
  std::vector<std::string> res;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    res.push_back(token);
    s = s.substr(pos + delimiter.length());
  }
  res.push_back(s);
  return res;
}
std::string find_arg(std::string &s,const std::string &arg_mark)
{
  size_t pos_mark=0,pos_end=0;
  std::string arg=s;
  if((pos_mark=arg.find(arg_mark))==std::string::npos) return "";
  arg=arg.substr(pos_mark+arg_mark.length());
  trim(arg);
  if((pos_end=arg.find(" "))!=std::string::npos)
  {
    arg=arg.substr(0,pos_end);
    s.erase(pos_mark,pos_end-pos_mark+1);
  }
  else
    s.erase(pos_mark,s.length()-pos_mark);
  return arg;
}
void trim(std::string &s)
{
  if(s.empty()) return;
  s.erase(0,s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ")+1);
  return;
}
int command_pipe(std::string cmd,int in_fd,int out_fd) //process external commands only with pipe
{
  trim(cmd);
  std::vector<std::string> args = split(cmd, " ");
  int pfd1[2],pfd2[2];
  int pipeturn=0; //0 represents pfd1 is for reading.
  int infd_backup=1000,outfd_backup=1001;
  dup2(in_fd,infd_backup);
  dup2(out_fd,outfd_backup);
  std::vector<std::string> subprocs=split(cmd,"|");
  if(subprocs.size()==1)
  {
    // 外部命令
    pid_t pid = fork();

    // std::vector<std::string> 转 char **
    char *arg_ptrs[args.size() + 1];
    for (auto i = 0; i < args.size(); i++) {
      arg_ptrs[i] = &args[i][0];
    }
    // exec p 系列的 argv 需要以 nullptr 结尾
    arg_ptrs[args.size()] = nullptr;

    if (pid == 0) {
      // 这里只有子进程才会进入
      // execvp 会完全更换子进程接下来的代码，所以正常情况下 execvp 之后这里的代码就没意义了
      // 如果 execvp 之后的代码被运行了，那就是 execvp 出问题了
      execvp(args[0].c_str(), arg_ptrs);

      // 所以这里直接报错
      exit(255);
    }

    // 这里只有父进程（原进程）才会进入
    int ret = wait(nullptr);
    if (ret < 0) {
      std::cout << "wait failed";
    }
  }
  else
  {
    close(STDOUT_FILENO);
    pipeturn=0;
    if(pipe(pfd1)==-1||pipe(pfd2)==-1)
    {
      std::cout<<"Pipe error\n";
      return -1;
    }
    for(int i=0;i<subprocs.size();i++)
    {
      std::string subproc=subprocs[i];
      trim(subproc);
      args = split(subproc, " ");
      // std::vector<std::string> 转 char **
      char *arg_ptrs[args.size() + 1];
      for (auto j = 0; j < args.size(); j++) {
        arg_ptrs[j] = &args[j][0];
      }
      // exec p 系列的 argv 需要以 nullptr 结尾
      arg_ptrs[args.size()] = nullptr;
      if(pipeturn==0)
      {
        close(pfd2[0]);
        close(pfd2[1]);
        if(pipe(pfd2)==-1)
        {
          return -1;
        }
      }
      else
      {
        close(pfd1[0]);
        close(pfd1[1]);
        if(pipe(pfd1)==-1)
        {
          return -1;
        }
      }
      pid_t pid = fork();
      if (pid == 0) {
        // 这里只有子进程才会进入
        if(pipeturn==0) //regard pfd1 or in_fd as input
        {
          if(i==0) //Read from stdin,write to pipe
          {
              close(pfd2[0]);
              dup2(pfd2[1],out_fd);
          }
          else if(i!=subprocs.size()-1) //Read from pipe,write to another pipe
          {
            close(pfd1[1]);
            dup2(pfd1[0],in_fd);
            close(pfd2[0]);
            dup2(pfd2[1],out_fd);
          }
          else //Read from pipe,write to out_fd
          {
            close(pfd1[1]);
            dup2(pfd1[0],in_fd);
            dup2(outfd_backup,out_fd);
          }
        }
        else //regard pfd2 or stdin as input
        {
          if(i==0) //Read from in_fd,write to pipe
          {
              close(pfd1[0]);
              dup2(pfd1[1],out_fd);
          }
          else if(i!=subprocs.size()-1) //Read from pipe,write to another pipe
          {
            close(pfd2[1]);
            dup2(pfd2[0],in_fd);
            close(pfd1[0]);
            dup2(pfd1[1],out_fd);
          }
          else //Read from pipe,write to out_fd
          {
            close(pfd2[1]);
            dup2(pfd2[0],in_fd);
            dup2(outfd_backup,out_fd);
          }
        }
        // execvp 会完全更换子进程接下来的代码，所以正常情况下 execvp 之后这里的代码就没意义了
        // 如果 execvp 之后的代码被运行了，那就是 execvp 出问题了
        execvp(args[0].c_str(), arg_ptrs);

        // 所以这里直接报错
        exit(255);
      }
      // 这里只有父进程（原进程）才会进入
      int ret = wait(nullptr);
      close(pfd1[1]);
      close(pfd2[1]);
      pipeturn=pipeturn?0:1; //Reverse turn
      if (ret < 0) {
        std::cout << "wait failed";
      }
    }
  }
  dup2(infd_backup,in_fd);
  dup2(outfd_backup,out_fd);
  return 0;
}