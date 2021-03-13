#include "ast.hpp"
#include "config.hpp"

void lexer2(std::string s, std::vector<TOKEN>& vtoken)
{
	unsigned int idx = 0;
	unsigned int bgn = 0;
	TOKEN_TYPE tt;

  while (idx < s.size())
  {
		/* 空白文字の処理 */
		if(s[idx] <= ' '){
			idx++;
			continue;
		}

		/* Token */
		bgn = idx;
		/* [a-zA-Z0-9/.]* */
		if(isalpha(s[idx]) || isdigit(s[idx]) || s[idx] == '/' || s[idx] == '.' || s[idx] == '_' || s[idx] == '*' || s[idx] == ':' || s[idx] == '-'){
			while(isalpha(s[idx]) || isdigit(s[idx]) || s[idx] == '/' || s[idx] == '.' || s[idx] == '_' || s[idx] == '*' || s[idx] == ':' || s[idx] == '-'){
				idx++;
			}
			tt = CHAR;
		}
		/* それ以外 */
		else {
			switch(s[idx]){
			case ';':
				tt = SEMICOLON;
				break;
			case '{':
				tt = LB;
				break;
			case '}':
				tt = RB;
				break;
			default:
				tt = UNAUTHORIZED;
				break;
			}	
			idx++;
		}
		
		TOKEN t;
    t.token = s.substr(bgn, idx - bgn);
		t.type = tt;
		vtoken.push_back(t);
	}
	return ;
}

void print_token2(std::vector<TOKEN> vtoken)
{
	for(std::vector<TOKEN>::iterator itr = vtoken.begin(); itr != vtoken.end(); ++itr) {
		if(itr->type != INITIAL){
			printf("TOKEN TYPE: ");
			switch(itr->type){
			case CHAR:
				printf("CHAR         ");
				break;
			case SEMICOLON:
				printf("SEMICOLON    ");
				break;
			case LB:
				printf("LB           ");
				break;
			case RB:
				printf("RB           ");
				break;
			default:
				printf("UNAUTHORIZED ");
			}
			printf("TOKEN: %s", itr->token.c_str());
		}
		printf("\n");
	}
}

unsigned int isContext(std::vector<TOKEN>& vtoken, unsigned int idx)
{
  unsigned int i = 0;

  while (vtoken.at(idx).type == CHAR)
  {
    i++;
    idx++;
  }
  if (i <= 1)
    return 0;
  if (vtoken.at(idx).type == SEMICOLON)
  {
    i++;
    return i;
  }
  return 0;
}

bool isModule(std::vector<TOKEN> vtoken, unsigned int& idx)
{
  static int brackets = 0;
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == LB)
    {
      brackets++;
      idx += 2;
      if (!isModule(vtoken, idx))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == CHAR && vtoken.at(idx + 2).type == LB)
    {
      brackets++;
      idx += 3;
      if (!isModule(vtoken, idx))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == RB)
    {
      brackets--;
      idx += 1;
      continue;
    }
    return false;
  }
  if (brackets != 0)
    return false;
  return true;
}

void pushContext(std::vector<TOKEN>& vtoken, unsigned int idx, configBase& config)
{
  std::string key = vtoken.at(idx++).token;
  for(std::vector<context>::iterator itr = 
     config.contexts.begin(); itr != config.contexts.end(); ++itr) {
    if (key == itr->key)
    {
      values values;
      while (vtoken.at(idx).type == CHAR)
      {
        values.value.push_back(vtoken.at(idx).token);
        idx++;
      }
      itr->values.push_back(values);
      return ;
    }
  }
  context context;
  context.key = key;
  values values;
  while (vtoken.at(idx).type == CHAR)
  {
    values.value.push_back(vtoken.at(idx).token);
    idx++;
  }
  context.values.push_back(values);
  config.contexts.push_back(context);
}

bool pushModule(std::vector<TOKEN> vtoken, unsigned int& idx, configBase& config)
{
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, config);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == LB)
    {
      idx += 2;
      if (!pushModule(vtoken, idx, config))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == CHAR && vtoken.at(idx + 2).type == LB)
    {
      idx += 3;
      if (!pushModule(vtoken, idx, config))
        return false;
      continue;
    }
    if (vtoken.at(idx).type == RB)
    {
      idx += 1;
      continue;
    }
    return false;
  }
  return true;
}

bool pushModuleToLocation(std::vector<TOKEN> vtoken, unsigned int& idx, configLocation& configLocation)
{
  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configLocation);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == RB)
    {
      idx += 1;
      return true;
    }
    return false;
  }
  return true;
}

bool pushModuleToServer(std::vector<TOKEN> vtoken, unsigned int& idx, configServer& configServer)
{
  unsigned int locations = 0;

  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configServer);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == CHAR && vtoken.at(idx + 2).type == LB)
    {
      std::string path = vtoken.at(idx + 1).token;
      for(std::vector<configLocation>::iterator itr = 
          configServer.locations.begin(); itr != configServer.locations.end(); ++itr) {
        if (itr->path == path)
          return false;
      }
      configLocation tmp;
      tmp.path = vtoken.at(idx + 1).token;
      configServer.locations.push_back(tmp);
      idx += 3;
      if (!pushModuleToLocation(vtoken, idx, configServer.locations[configServer.locations.size() - 1]))
        return false;
      locations++;
      continue;
    }
    if (vtoken.at(idx).type == RB)
    {
      idx += 1;
      return true;
    }
    return false;
  }
  return true;
}

bool pushModuleToHttp(std::vector<TOKEN> vtoken, unsigned int& idx, configHttp& configHttp)
{
  unsigned int servers = 0;

  while (idx < vtoken.size())
  {
    if (unsigned int r = isContext(vtoken, idx))
    {
      pushContext(vtoken, idx, configHttp);
      idx += r;
      continue;
    }
    if (vtoken.at(idx).type == CHAR && vtoken.at(idx + 1).type == LB)
    {
      idx += 2;
      configServer tmp;
      configHttp.servers.push_back(tmp);
      if (!pushModuleToServer(vtoken, idx, configHttp.servers.at(servers)))
        return false;
      servers++;
      continue;
    }
    return false;
  }
  return true;
}

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
    char buf[1024];
    i_for_buf = 0;
    while (i_for_buf < 1024)
      buf[i_for_buf++] = 0;
    while ((r = read(fd, buf, 1024)) > 0)
    {
      cmd.append(buf);
      i_for_buf = 0;
      while (i_for_buf < 1024)
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
    cmd = "client_max_body_size 1048576;error_page 400 403 404 /40x.html;server {  listen *:80;  error_page 400 403 404 /40x.html;  root /home/user42/www;    location / {    allow_methods GET POST PUT DELETE;    autoindex on;    index index.html;    cgi-script .cgi .php;  }    }";
  }
  else
  {
    std::cerr << "There are a large number of arguments." << std::endl;
    exit(1);
  }
  std::cout << "[" << cmd << "]" << std::endl;

  std::vector<TOKEN> vtoken;

  lexer2(cmd, vtoken);
  print_token2(vtoken);
  std::cout << std::endl;

  configHttp config;
  unsigned int idx = 0;
  std::cout << "設定ファイルの文法チェック" << std::endl;
  r = isModule(vtoken, idx);
  if (r == 0)
  {
    std::cerr << "NG" << std::endl;
    exit(1);
  }
  else
    std::cerr << "OK" << std::endl;
  std::cout << std::endl;

  configHttp configHttp_002;
  idx = 0;
  std::cout << "設定ファイルの内容をclassに格納" << std::endl;
  r = pushModuleToHttp(vtoken, idx, configHttp_002);
  if (r == 0)
  {
    std::cerr << "NG" << std::endl;
    exit(1);
  }
  else
    std::cerr << "OK" << std::endl;
  std::cout << std::endl;

  std::cout << "設定内容" << std::endl;
  std::cout << "グローバル設定" << std::endl;
  for(std::vector<context>::iterator itr = 
      configHttp_002.contexts.begin(); itr != configHttp_002.contexts.end(); ++itr) {
    std::cout << "  key = " << itr->key << std::endl;
    for(std::vector<values>::iterator itrValues = 
        itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
      std::cout << "    value = ";
      for(std::vector<std::string>::iterator itrValue = 
          itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
        std::cout << *itrValue << " ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  unsigned int i = 1;
  for(std::vector<configServer>::iterator itrServer = 
      configHttp_002.servers.begin(); itrServer != configHttp_002.servers.end(); ++itrServer) {
    std::cout << "サーバー設定(" << i++ << ")" << std::endl;
    for(std::vector<context>::iterator itr =
        itrServer->contexts.begin(); itr != itrServer->contexts.end(); ++itr) {
      std::cout << "  key = " << itr->key << std::endl;
      for(std::vector<values>::iterator itrValues = 
          itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
        std::cout << "    value = ";
        for(std::vector<std::string>::iterator itrValue = 
            itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
          std::cout << *itrValue << " ";
        }
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    for(std::vector<configLocation>::iterator itrConfig =
        itrServer->locations.begin(); itrConfig != itrServer->locations.end(); ++itrConfig) {
      std::cout << "  ロケーション設定(" << itrConfig->path << ")" << std::endl;
      for(std::vector<context>::iterator itr = 
          itrConfig->contexts.begin(); itr != itrConfig->contexts.end(); ++itr) {
        std::cout << "    key = " << itr->key << std::endl;
        for(std::vector<values>::iterator itrValues = 
            itr->values.begin(); itrValues != itr->values.end(); ++itrValues) {
          std::cout << "      value = ";
          for(std::vector<std::string>::iterator itrValue = 
              itrValues->value.begin(); itrValue != itrValues->value.end(); ++itrValue) {
            std::cout << *itrValue << " ";
          }
          std::cout << std::endl;
        }
      }
      std::cout << std::endl;
    }
  }

	return EXIT_SUCCESS;
}