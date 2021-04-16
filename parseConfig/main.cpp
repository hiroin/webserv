#include "parseConfig.hpp"
#include "Config.hpp"

int main(int argc, char* argv[])
{
  int i_for_buf;
  int r;
  int fd;
  std::string cmd;

  if (argc == 2)
  {
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
      std::cerr << "Failed to open the file." << std::endl;
      std::cerr << strerror(errno) << std::endl; 
      exit(1);
    }
    char buf[2048];
    i_for_buf = 0;
    while (i_for_buf < 2048)
      buf[i_for_buf++] = 0;
    while ((r = read(fd, buf, 2048)) > 0)
    {
      cmd.append(buf);
      i_for_buf = 0;
      while (i_for_buf < 2048)
        buf[i_for_buf++] = 0;
    }
    if (r == -1)
    {
      std::cerr << "Failed to load the file." << std::endl;
      std::cerr << strerror(errno) << std::endl; 
      exit(1);
    }
    r = close(fd);
    if (r == -1)
    {
      std::cerr << "Failed to close the file." << std::endl;
      std::cerr << strerror(errno) << std::endl; 
      exit(1);
    }
  }
  else if (argc == 1)
  {
    cmd = "client_max_body_size 1048576;error_page 400 403 404 /40x.html;php-cgi-path /usr/bin/php-cgi;server {  listen *:80;  error_page 400 403 404 /40x.html;  root /home/user42/www;    location / {    allow_methods GET POST PUT DELETE;    autoindex on;    index index.html;    cgi-script .cgi .php;  }    }";
  }
  else
  {
    std::cerr << "There are a large number of arguments." << std::endl;
    exit(1);
  }
  // デバッグ用出力
  // configの内容表示
  // std::cout << "[" << cmd << "]" << std::endl;

  parseConfig pc;
  try
  {
    pc.lexer(cmd);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
  // デバッグ用出力
  // pc.printToken();
  // std::cout << std::endl;
  // return 0;

  {
    std::cout << "設定ファイルの文法チェック : ";
    unsigned int idx = 0;
    r = pc.checkFormat();
    if (r == 0)
    {
      std::cout << "NG" << std::endl;
      exit(1);
    }
    else
      std::cout << "OK" << std::endl;
    std::cout << std::endl;
  }

  std::cout << "設定ファイルの内容を一時作業領域に格納 : ";
  r = pc.insertConfigToTmpMemory();
  if (r == 0)
  {
    std::cerr << "NG" << std::endl;
    exit(1);
  }
  else
    std::cerr << "OK" << std::endl;
  std::cout << std::endl;

  // デバッグ用出力
  pc.printConfigHttp();
  std::cout << "-------------------------------------" << std::endl;

  // Configクラスにデータの挿入
  Config c;
  try
  {
    pc.insertToConfigClass(c);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit(1);
  }
  c.printConfig();
	return EXIT_SUCCESS;
}