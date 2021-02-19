#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <errno.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[])
{
  int pid;
  int p[2];
  char buf[1024];
  ssize_t count;

  pipe(p);
  pid = fork(); 
  if (pid == 0) {
    close(p[0]);
    dup2(p[1], 1);
    close(p[1]);
    char *argv[2];
    argv[0] = strdup("/bin/ls");
    argv[1] = NULL;
    (void) execve(argv[0], argv, envp);
  }
  (void) close(p[1]);
  count = read(p[0], buf, 1024);
  std::cout << "count : " << count << std::endl;    return 0;
}

