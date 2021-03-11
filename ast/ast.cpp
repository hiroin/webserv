#include "ast.hpp"
#include "config.hpp"

typedef enum _token_type {
	INITIAL,
	CHAR,     /* 文字 */
	SEMICOLON,/* ; */
	LB,       /* { */
	RB,       /* } */
  UNAUTHORIZED,
	END
} TOKEN_TYPE;

class TOKEN {
 public:
	TOKEN_TYPE    type;
	std::string   token;
	TOKEN *next;
	TOKEN *prev;
} ;

void lexer(std::string s, TOKEN *token)
{
	unsigned int idx = 0;
	unsigned int bgn = 0;
	TOKEN *t;
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
		if(isalpha(s[idx]) || isdigit(s[idx]) || s[idx] == '/' || s[idx] == '.' || s[idx] == '_' || s[idx] == '*' || s[idx] == ':'){
			while(isalpha(s[idx]) || isdigit(s[idx]) || s[idx] == '/' || s[idx] == '.' || s[idx] == '_' || s[idx] == '*' || s[idx] == ':'){
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
		
		t = new TOKEN();
    t->token = s.substr(bgn, idx - bgn);
		t->type = tt;
		t->next = NULL;
		token->next = t;
		t->prev = token;
		token = t;
	}
	return ;
}

void print_token(TOKEN* token)
{
	while(1) {
		if(token->type == END){
			break;
		}
		if(token->type != INITIAL){
			printf("TOKEN TYPE: ");
			switch(token->type){
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
			printf("TOKEN: %s", token->token.c_str());
		}

		printf("\n");
		if(token->next == NULL){
			break;
		}
		token  = token->next;
	}
}

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
      // std::cout << vtoken.at(idx).token << std::endl;
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

  std::vector<std::string> values;
  while (vtoken.at(idx).type == CHAR)
  {
    values.push_back(vtoken.at(idx).token);
    idx++;
  }
  config.contexts.insert(std::make_pair(key, values));
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
      std::string key = vtoken.at(idx + 1).token;
      std::map<std::string, configLocation>::iterator itr =
          configServer.locations.find(key);
      if (itr == configServer.locations.end())
      {
        configLocation tmp;
        configServer.locations[vtoken.at(idx + 1).token] = tmp;
      }
      idx += 3;
      if (!pushModuleToLocation(vtoken, idx, configServer.locations[key]))
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
	// TOKEN *o_token;
  // TOKEN *token;
  // std::string cmd = "client_max_body_size 104876;  error_page           400 403 404 /40x.html;  server {    listen  *:8080;    root    /home/user42/www8080/;  }";
  std::string cmd = "client_max_body_size 1048576;error_page 400 403 404 /40x.html;server {  listen *:80;  error_page 400 403 404 /40x.html;  root /home/user42/www;    location / {    allow_methods GET POST PUT DELETE;    autoindex on;    index index.html;    cgi-script .cgi .php;  }    }";
  std::vector<TOKEN> vtoken;

  // token = (TOKEN*)malloc(sizeof(TOKEN));
  // o_token = token;
  // token->type = INITIAL;
  // token->prev = NULL;
  // token->next = NULL;
  
  // lexer(cmd, token);
  // print_token(o_token);

  lexer2(cmd, vtoken);
  print_token2(vtoken);

  configHttp config;
  unsigned int idx = 0;
  std::cout << isModule(vtoken, idx) << std::endl;
  std::cout << std::endl;

  configHttp configHttp_001;
  idx = 0;
  std::cout << pushModule(vtoken, idx, configHttp_001) << std::endl;
  for(std::map<std::string, std::vector<std::string> >::iterator itr = 
      configHttp_001.contexts.begin(); itr != configHttp_001.contexts.end(); ++itr) {
    std::cout << "key = " << itr->first << ", value = " << itr->second.at(0) << std::endl;
  }
  std::cout << std::endl;

  configHttp configHttp_002;
  idx = 0;
  std::cout << pushModuleToHttp(vtoken, idx, configHttp_002) << std::endl;
  for(std::map<std::string, std::vector<std::string> >::iterator itr = 
      configHttp_002.contexts.begin(); itr != configHttp_002.contexts.end(); ++itr) {
    std::cout << "key = " << itr->first << ", value = " << itr->second.at(0) << std::endl;
  }
  for(std::map<std::string, std::vector<std::string> >::iterator itr = 
      configHttp_002.servers.at(0).contexts.begin(); itr != configHttp_002.servers.at(0).contexts.end(); ++itr) {
    std::cout << "key = " << itr->first << ", value = " << itr->second.at(0) << std::endl;
  }
  for(std::map<std::string, std::vector<std::string> >::iterator itr = 
      configHttp_002.servers.at(0).locations["/"].contexts.begin();
      itr != configHttp_002.servers.at(0).locations["/"].contexts.end(); ++itr) {
    std::cout << "key = " << itr->first << ", value = " << itr->second.at(0) << std::endl;
  }

  // token = o_token;

	return EXIT_SUCCESS;
}