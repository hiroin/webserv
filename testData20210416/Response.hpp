

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

struct ResponseHeaderField
{
	std::string Location; /*サーバーでリダイレクトされるパス*/
	std::string Retry_After; /*いつ送信し直すか*/
	std::string Server;
	std::string WWW_Authenticate; /* 401 Unauthorized 応答と共に送られる
									ベーシック認証ができるかどうかをチェック?*/
};

struct GeneralHeaderField
{
	std::string Date;
	std::string Transfer_Encoding; /*どうやって決めよう*/

};

struct EntityHeaderField
{
	std::string Allow;
	std::string Content_Language;
	std::string Content_Length;
	std::string Content_Location;
	std::string Content_Type;
	std::string Last_Modified;
};

struct ResponseInfo
{
	std::string RequestLine;
	ResponseHeaderField rhf;
	GeneralHeaderField ghf;
	EntityHeaderField ehf;
	std::string MessageBody;
};
/**とりあえず、200 ok をかえせるように**/
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
		// std::string errorFilePath;
		/**
		 * Accept-Language
		 * **/

		/**
		 * エラーページがあったら出せる。なかったらエラーステータスだけを返すようにする。
		 * **/

		/*各レスポンスで適用する設定項目*/

		/*
			曜日を取得する関数
		*/
		std::map<int, std::string> GetDate();

		/**
		 * 月を取得する関数
		 * **/
		std::map<int, std::string> GetMonth();

		std::map<std::string, std::vector<std::string> > AcceptLanguageMap;

		/**
		 * クライアントと、リクエストからどのサーバーを使うのかきめる
		* **/
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
		int getTargetFileFd();
		bool isMethodAllowed();
		void setAllow();
		s_ConfigCommon getConfigCommon();
		bool isMatchAcceptLanguageFromat(std::string src);
		bool isAcceptLanguageSet();
		int isLanguageFileExist(std::string SerachFileAbsolutePath);
		void setContentLanguage();
		bool isLanguageFile(std::string FilePath, std::string sub);
		/*.<Accept-Language> がなかったら406を返す*/
		std::map<std::string, std::vector<std::string> > parseAcceptLanguage(std::string src);
		bool isAuthorized();
		void AppendBodyOnResponseMessage(std::string body);
		bool isNecesarryAuth();
		bool isRequestMatchAuth();
		std::string getEncodedServerCredential();
		void setWWWAuthenticate();

		/*===============HTTPstatus===============*/


		/*===============ResponseHeaderField===============*/
		/**
		 * Location ヘッダを作る関数
		 * **/


		/**
		 * Serverヘッダをつくる関数
		 * **/
		void SetServer();
		/*===============EntityHeaderField===============*/
		/**
		 * Aloowヘッダを作る関数
		 * **/
		void SetAllow();
		/*===============GlobalHeaderField===============*/

		/**
		 *  Dateヘッダをつくるための関数
		 * **/
		void SetDate();

		/*=====================返却用ファイルを入手する部分========================*/
		std::string MakeFilePath();
		std::string Getfile(std::string path);

		bool isHTTPMethodHEAD();

		int send();
		Response(int test_number);
		Response(Client &client, Config &config);
		Response(int ErrorCode ,Client &client, Config &config);
		~Response();
};
#endif /* A9308F37_DB41_4E16_8DFF_32241C903504 */
