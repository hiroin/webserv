

#ifndef A9308F37_DB41_4E16_8DFF_32241C903504
#define A9308F37_DB41_4E16_8DFF_32241C903504

#include <string>
#include <vector>
#include <time.h>
#include "Config.hpp"
#include "HTTPMessageParser.hpp"
#include "Client.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


class Response
{

	public:
		int ResponseStatus;
		s_ConfigServer configServer;
		s_ConfigLocation configLocation;
		/* コンストラクタで外部から初期化するメンバ */
		Config &config;
		Client &client;

		std::string responseMessege;
		std::string targetFilePath;
		std::string AutoIndexContent;
		bool isAutoIndexApply;
		bool isCGI;
		int readFd;
		int writeFd;


		std::map<std::string, std::vector<std::string> > AcceptLanguageMap;
		std::map<std::string, std::vector<std::string> > AcceptCharsetMap;
		std::string PutPostBody;
		std::vector<std::string> envp;
		std::vector<std::string> argv;
		std::map<int, std::string> GetDate();
		std::map<int, std::string> GetMonth();
		bool DecideConfigServer(); //使うサーバーを決定
		bool DecideConfigLocation(); //使うlocation を決定
		std::string GetSerachAbsolutePath(); //Requestのパスと、
		void setTargetFileAndStatus();
		int isTargetFileAbailable(std::string);
		size_t getContentLength();
		void setContentLength();
		void setResponseLine();
		void setDate();
		bool isErrorFilePathExist();
		void setContenType();
		std::string GetContentType(std::string key); //contentTypeを取得する関数
		std::string GetLastModified(); //最終更新日を返す関数
		bool isMethodAllowed();
		void setAllow();
		s_ConfigCommon getConfigCommon();
		bool isMatchAcceptLanguageFromat(std::string);
		bool isAcceptLanguageSet();
		int isLanguageFileExist(std::string);
		void setContentLanguage();
		bool isLanguageFile(std::string, std::string);
		std::map<std::string, std::vector<std::string> > parseAcceptLanguage(std::string src);
		bool isAuthorized();
		void AppendBodyOnResponseMessage(std::string body);
		bool isNecesarryAuth();
		bool isRequestMatchAuth();
		std::string getEncodedServerCredential();
		void setWWWAuthenticate();
		bool isAcceptCharsetSet();
		bool isMatchAcceptCharsetFromat(std::string);
		std::map<std::string, std::vector<std::string> > parseAcceptCharset(std::string src);
		int isCharsetFileExist(std::string);
		std::string removeLanguageAndCharsetFileExtention();
		bool isCharsetFile(std::string FilePath, std::string fileExtention);
		int isCharsetAndLanguageFileExist(std::string SerachFileAbsolutePath);
		std::string getCharsetExtention();
		bool isAutoIndex();
		bool isDirectoryAvailable();
		void getAutoIndexContent();
		bool isDirectory(std::string name);
		std::vector<std::string> getDirectoryContents();
		std::string makeATag(std::string dataName);
		void	makeAutoIndexResponse();
		bool	isTransferEncodingChunked();
		bool	isContentLength();
		int		getFileFdForWrite();
		void	setLocation();
		void	addSlashOnAbsolutePath();
		void SetServer();
		void SetAllow();
		void SetDate();
		std::string MakeFilePath();
		std::string Getfile(std::string path);
		bool isHTTPMethodHEAD();
		int send();
		bool isCgiFile();
		bool isReadable(std::string filePath);
		bool isExecutable(std::string filePath);
		void addEnvironmentValue();
		int getCgiFd(); //CGIの読み取り用FDを取得する関数
		int getTargetFileFd(); //ファイル読み取り用FDを取得する関数
		void mergeCgiResult(std::string CgiResult);

		//php-cgi
		bool execPhpCgi();
		/////////////////////////////////

		bool execCgi();
		Response(int test_number);
		Response(Client &client, Config &config);
		Response(int ErrorCode ,Client &client, Config &config);
		~Response();
};

#endif /* A9308F37_DB41_4E16_8DFF_32241C903504 */

// /* 残タスク　HEAD, POST が '/' で終わってる場合には、'/' を rootとしてファイルをアップロードする必要あり config に uploadPath があるので、そのpath にアップロード、ファイル名は適当に決める。*/
