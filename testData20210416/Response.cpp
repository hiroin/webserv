#include "Response.hpp"
#include "Config.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <errno.h>
#include <ctime>
#include <algorithm>

int ft_pow(int n, int times)
{
	if (times == 0)
	{
		return (1);
	}
	for(int i = 0; i < times; i++)
	{
		n *= n;
	}
	return (n);
}

std::string ft_itos(int nu)
{
	std::string ret;
	while (nu > 0)
	{
		char c[2];
		c[0] = '0' + nu % 10;
		c[1] = '\0';
		ret.insert(0, std::string(c));
		nu /= 10;
	}
	return (ret);

}

std::string ft_ltos(long nu)
{
	std::string ret;
	while (nu > 0)
	{
		char c[2];
		c[0] = '0' + nu % 10;
		c[1] = '\0';
		ret.insert(0, std::string(c));
		nu /= 10;
	}
	return (ret);

}


void setResponseMap(std::map<int, std::string> &ResponseMap)
{
	ResponseMap[100] =  "Continue";
	ResponseMap[101] =  "Switching Protocols";
	ResponseMap[103] =  "Early Hints";
	ResponseMap[200] =  "OK";
	ResponseMap[201] =  "Created";
	ResponseMap[202] =  "Accepted";
	ResponseMap[203] =  "Non-Authoritative Information";
	ResponseMap[204] =  "No Content";
	ResponseMap[205] =  "Reset Content";
	ResponseMap[206] =  "Partial Content";
	ResponseMap[300] =  "Multiple Choices";
	ResponseMap[301] =  "Moved Permanently";
	ResponseMap[302] =  "Found";
	ResponseMap[303] =  "See Other";
	ResponseMap[304] =  "Not Modified";
	ResponseMap[307] =  "Temporary Redirect";
	ResponseMap[308] =  "Permanent Redirect";
	ResponseMap[400] =  "Bad Request";
	ResponseMap[401] =  "Unauthorized";
	ResponseMap[402] =  "Payment Required";
	ResponseMap[403] =  "Forbidden";
	ResponseMap[404] =  "Not Found";
	ResponseMap[405] =  "Method Not Allowed";
	ResponseMap[406] =  "Not Acceptable";
	ResponseMap[407] =  "Proxy Authentication Required";
	ResponseMap[408] =  "Request Timeout";
	ResponseMap[409] =  "Conflict";
	ResponseMap[410] =  "Gone";
	ResponseMap[411] =  "Length Required";
	ResponseMap[412] =  "Precondition Failed";
	ResponseMap[413] =  "Payload Too Large";
	ResponseMap[414] =  "URI Too Long";
	ResponseMap[415] =  "Unsupported Media Type";
	ResponseMap[416] =  "Range Not Satisfiable";
	ResponseMap[417] =  "Expectation Failed";
	ResponseMap[418] =  "I'm a teapot";
	ResponseMap[422] =  "Unprocessable Entity";
	ResponseMap[425] =  "Too Early";
	ResponseMap[426] =  "Upgrade Required";
	ResponseMap[428] =  "Precondition Required";
	ResponseMap[429] =  "Too Many Requests";
	ResponseMap[431] =  "Request Header Fields Too Large";;
	ResponseMap[451] =  "Unavailable For Legal Reasons";
	ResponseMap[500] =  "Internal Server Error";
	ResponseMap[501] =  "Not Implemented";
	ResponseMap[502] =  "Bad Gateway";
	ResponseMap[503] =  "Service Unavailable";
	ResponseMap[504] =  "Gateway Timeout";
	ResponseMap[505] =  "HTTP Version Not Supported";
	ResponseMap[506] =  "Variant Also Negotiates";
	ResponseMap[507] =  "Insufficient Storage";
	ResponseMap[508] =  "Loop Detected";
	ResponseMap[510] =  "Not Extended";
	ResponseMap[511] =  "Network Authentication";
}

std::map<int, std::string> Response::GetDate()
{
	std::map<int, std::string> date;
	date[0] = "Sun";
	date[1] = "Mon";
	date[2] = "Tue";
	date[3] = "Wed";
	date[4] = "Thu";
	date[5] = "Fri";
	date[6] = "Sat";
	return (date);
}

std::map<int, std::string> Response::GetMonth()
{
	std::map<int, std::string> month;
	month[0] = "Jan";
	month[1] = "Feb";
	month[2] = "Mar";
	month[3] = "Apr";
	month[4] = "May";
	month[5] = "Jun";
	month[6] = "Jul";
	month[7] = "Aug";
	month[8] = "Sep";
	month[9] = "Oct";
	month[10] = "Nov";
	month[11] = "Dec";
	return (month);
}

std::string Response::GetContentType(std::string key)
{
	std::map<std::string, std::string> TypeMap;

TypeMap[".aac"]    = "audio/aac";                              //AAC 音声
TypeMap[".abw"]    = "application/x-abiword";                  //AbiWord文書
TypeMap[".arc"]    = "application/x-freearc";                  //(複数のファイルが埋め込まれた) アーカイブ文書
TypeMap[".avi"]    = "video/x-msvideo";                        //AVI: Audio Video Interleave
TypeMap[".azw"]    = "application/vnd.amazon.ebook";           //Amazon Kindle eBook 形式
TypeMap[".bin"]    = "application/octet-stream";               //任意の種類のバイナリーデータ
TypeMap[".bmp"]    = "image/bmp";                              //Windows OS/2 ビットマップ画像
TypeMap[".bz"]     = "application/x-bzip";                     //BZip アーカイブ
TypeMap[".bz2"]    = "application/x-bzip2";                    //BZip2 アーカイブ
TypeMap[".csh"]    = "application/x-csh";                      //C-Shell スクリプト
TypeMap[".css"]    = "text/css";                               //カスケーディングスタイルシート (CSS)
TypeMap[".csv"]    = "text/csv";                               //カンマ区切り値 (CSV)
TypeMap[".doc"]    = "application/msword";                     //Microsoft Word
TypeMap[".docx"]   = "application/vnd.openxmlformats-officedocument.wordprocessingml.document"; // Microsoft Word (OpenXML)
TypeMap[".eot"]    = "application/vnd.ms-fontobject";          //MS 埋め込み OpenType フォント
TypeMap[".epub"]   = "application/epub+zip";                   //電子出版 (EPUB)
TypeMap[".gz"]     = "application/gzip";                       //GZip 圧縮アーカイブ
TypeMap[".gif"]    = "image/gif";                              //グラフィック交換形式 (GIF)
TypeMap[".htm"]    = "text/html";                              //ハイパーテキストマークアップ言語 (HTML)
TypeMap[".html"]   = "text/html";                              //ハイパーテキストマークアップ言語 (HTML)
TypeMap[".ico"]    = "image/vnd.microsoft.icon";               //アイコン形式
TypeMap[".ics"]    = "text/calendar";                          //iCalendar 形式
TypeMap[".jar"]    = "Java Archive (JAR)";                     //application/java-archive
TypeMap[".jpeg"]   = "image/jpeg";                             //JPEG 画像
TypeMap[".jpg"]    = "image/jpeg";                             //JPEG 画像
TypeMap[".js"]     = "text/javascript";                        //JavaScript
TypeMap[".json"]   = "application/json";                       //JSON 形式
TypeMap[".jsonld"] = "application/ld+json";                    //JSON-LD 形式
TypeMap[".midi"]   = "audio/x-midi";                           //Musical Instrument Digital Interface (MIDI)
TypeMap[".mid"]    = "audio/midi";                             //Musical Instrument Digital Interface (MIDI)
TypeMap[".mjs"]    = "text/javascript";                        //JavaScript モジュール
TypeMap[".mp3"]    = "audio/mpeg";                             //MP3 音声
TypeMap[".mpeg"]   = "video/mpeg";                             //MPEG 動画
TypeMap[".mpkg"]   = "application/vnd.apple.installer+xml";    //Apple Installer Package
TypeMap[".odp"]    = "application/vnd.oasis.opendocument.presentation";    //OpenDocuemnt プレゼンテーション文書
TypeMap[".ods"]    = "application/vnd.oasis.opendocument.spreadsheet";     //OpenDocuemnt 表計算文書
TypeMap[".odt"]    = "application/vnd.oasis.opendocument.text";            //OpenDocument テキスト文書
TypeMap[".oga"]    = "audio/ogg";                              //OGG 音声
TypeMap[".ogv"]    = "video/ogg";                              //OGG 動画
TypeMap[".ogx"]    = "application/ogg";                        //OGG
TypeMap[".opus"]   = "audio/opus";                             //Opus 音声
TypeMap[".otf"]    = "font/otf";                               //OpenType フォント
TypeMap[".png"]    = "image/png";                              //Portable Network Graphics
TypeMap[".pdf"]    = "application/pdf";                        //Adobe Portable Document Format (PDF)
TypeMap[".php"]    = "application/x-httpd-php";                //Hypertext Preprocessor (Personal Home Page)
TypeMap[".ppt"]    = "application/vnd.ms-powerpoint";          //Microsoft PowerPoint
TypeMap[".pptx"]   = "application/vnd.openxmlformats-officedocument.presentationml.presentation"; //     Microsoft PowerPoint (OpenXML)
TypeMap[".rar"]    = "application/vnd.rar";                    //RAR アーカイブ
TypeMap[".rtf"]    = "application/rtf";                        //リッチテキスト形式 (RTF)
TypeMap[".sh"]     = "application/x-sh";                       //Bourne shell スクリプト
TypeMap[".svg"]    = "image/svg+xml";                          //Scalable Vector Graphics (SVG)
TypeMap[".swf"]    = "application/x-shockwave-flash";          //Small web format (SWF) または Adobe Flash 文書
TypeMap[".tar"]    = "application/x-tar";                      //Tape Archive (TAR)
TypeMap[".tif"]    = "image/tiff";                             //Tagged Image File Format (TIFF)
TypeMap[".tiff"]   = "image/tiff";                             //Tagged Image File Format (TIFF)
TypeMap[".ts"]     = "video/mp2t";                             //MPEG transport stream
TypeMap[".ttf"]    = "font/ttf";                               //TrueType フォント
TypeMap[".txt"]    = "text/plain";                             //テキストファイル (一般に ASCII or ISO 8859-<em>n</em>)
TypeMap[".vsd"]    = "application/vnd.visio";                  //Microsoft Visio
TypeMap[".wav"]    = "audio/wav";                              //Waveform 音声形式
TypeMap[".weba"]   = "audio/webm";                             //WEBM 音声
TypeMap[".webm"]   = "video/webm";                             //WEBM 動画
TypeMap[".webp"]   = "image/webp";                             //WEBP 画像
TypeMap[".woff"]   = "font/woff";                              //Web Open Font Format (WOFF)
TypeMap[".woff2"]  = "font/woff2";                             //Web Open Font Format (WOFF)
TypeMap[".xhtml"]  = "application/xhtml+xml";                  //XHTML
TypeMap[".xls"]    = "application/vnd.ms-excel";               //Microsoft Excel
TypeMap[".xlsx"]   = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
TypeMap[".xml"]    = "application/xml";                        //XML (一般のユーザから読める場合)
TypeMap[".xml"]    = "text/xml";                               //XML (一般のユーザから読めない場合)
TypeMap[".xul"]    = "application/vnd.mozilla.xul+xml";        //XUL
TypeMap[".zip"]    = "application/zip";                        //ZIP アーカイブ
TypeMap[".3gp"]    = "video/3gpp";                             //3GPP 音声/動画コンテナー
TypeMap[".3gp"]    = "audio/3gpp";                             //3GPP 音声/動画コンテナー (動画含まず)
TypeMap[".3g2"]    = "video/3gpp2";                            //3GPP2 音声/動画コンテナー
TypeMap[".3g2"]    = "audio/3gpp2";                            //3GPP2 音声/動画コンテナー (動画含まず)
TypeMap[".7z"]     = "application/x-7z-compressed";            //7-zipアーカイブ

	return (TypeMap[key]);
}

Response::~Response(){};


Response::Response(Client &client, Config &config) : client(client), config(config)
{
	DecideConfigServer(); //使用するserverディレクティブを決定
	DecideConfigLocation(); //使用するlocationディレクティブを決定
	setTargetFileAndStatus(); //探しにいくファイルパスと、レスポンスステータスを決定
	setResponseLine(); //responseStatus と serverNameヘッダを設定
	setDate(); //
	if (ResponseStatus != 200)
	{
		//erro_pageを探して、あったらContent-Type,Content-Lengthを記入して
		if (isErrorFilePathExist())
		{
			setContenType(errorFilePath);
			client.status = READ;
			return ;
		}
		else
		{
			client.status = SEND;
			return ;
		}
	}
	setContenType(targetFilePath);
	client.status = READ;
}

Response::Response(int ErrorCode ,Client &client, Config &config) : client(client), config(config)
{
	DecideConfigServer(); //使用するserverディレクティブを決定
	DecideConfigLocation(); //使用するlocationディレクティブを決定
	ResponseStatus = ErrorCode;
	setResponseLine(); //responseStatus と serverNameヘッダを設定
	setDate(); //Dateヘッダを設定
	if (isErrorFilePathExist())
	{
		if (isErrorFilePathExist())
		{
			setContenType(errorFilePath);
			client.status = READ;
			return ;
		}
		else
		{
			client.status = SEND;
			return ;
		}
	}
}


bool Response::DecideConfigServer()
{
	int clientPort = client.port_;
	std::string clientHostName = client.host_;
	HTTPMessageParser hmp = client.hmp_;

	//ここから、configをなめていって該当設定を見つけよう。
	std::vector<s_ConfigServer> servers = config.configGlobal.servers;
	for(int i = 0; i < servers.size(); i++)
	{
		if (servers[i].port == clientPort && servers[i].host == clientHostName) //クライアントとportが一致する設定は必ずある
		{
			std::vector<std::string> serverNames = servers[i].serverNames;
			std::string requestHOST = hmp.headers_["host"];
			for(int j = 0; j < serverNames.size(); j++)
			{
				if (serverNames[j] == requestHOST)
				{
					this->configServer = servers[i];
					return (true);
				}
			}
		}
	}
	for(int i = 0; i < servers.size(); i++)
	{
		if (servers[i].port == clientPort && servers[i].host == clientHostName)// serverNameで一致するものがなかったら一番上のもの
		{
					this->configServer = servers[i];
					return (true);
		}
	}
	return (true);
}

/**
 * リクエストをみて、Location を決定する。
 * **/

/**
 * location に該当するpath がなかったらどうする？ -> 404
 * location が
 * **/

//前方一致
//locationがなかったら、serverディレクティブ
bool Response::DecideConfigLocation()
{
	std::string AbsolutePath = this->client.hmp_.absolutePath_;
	std::vector<s_ConfigLocation> locations = this->configServer.locations;

	for(int i = 0; i < locations.size(); i++)// 文字が長い順番でlocationが入ってる。
	{
		if (AbsolutePath.find(locations[i].path) == 0) //location path と absolutePathが先頭一致した場合
		{
			configLocation = locations[i];
			return true;
		}
	}
	return false;
}

std::string Response::GetSerachAbsolutePath() //出来上がったpathに"/"が付いていなかったらindexを採用していく
{
	std::string SerachAbsolutePath = "";
	std::string AbsolutePath = client.hmp_.absolutePath_;
	if (configLocation.path.size() == 0 || configLocation.alias.size() == 0) //該当するLocationがなかったoraliasがなかった場合、rootが先頭につく
	{
		SerachAbsolutePath = configServer.root + AbsolutePath; //targetFilePath
	}
	else //locationがあれば、そのalisaを置換。
	{
		SerachAbsolutePath = AbsolutePath.replace(0, configLocation.path.size(), configLocation.alias);
	}
	return (SerachAbsolutePath);
}

int isTargetFileAbailable(std::string SerachFileAbsolutePath)
{
	struct stat buf;
	if(stat(SerachFileAbsolutePath.c_str(), &buf) == 0) //ファイルが存在して検索できた
	{
		return (200);
	}
	else //失敗したから、errnoチェックして確認
	{
		if(errno == ENOENT)
		{
			return (404);
		}
		return (403);
	}
	/**
	 * ステータスコードどうやって決めればいいかな....
	 * **/
}

void Response::setTargetFileAndStatus() //GetSerachAbsolutePath() が返してくる物をみて、ファイルがそもそも存在するかをチェック
{
	int statusNo;
	std::string targetFilePath;
	std::map<int, std::string> ret;
	std::string SerachFileAbsolutePath = GetSerachAbsolutePath();
	// std::cout << (SerachFileAbsolutePath[SerachFileAbsolutePath.size() - 1] == '\\') << std::endl;
	if (SerachFileAbsolutePath[SerachFileAbsolutePath.size() - 1] == '/')
	{
		std::vector<std::string> indexs;
		if (configLocation.path.size() == 0) //Locationにいない時
			indexs = configServer.configCommon.indexs; //serverディレクティブが採用されている
		else
			indexs = configLocation.configCommon.indexs; //Locationディレクティブが採用されている
		for(int i = 0; i < indexs.size(); i++)
		{
			targetFilePath = SerachFileAbsolutePath + indexs[i]; //indexファイルを見ていく
			int statusNo = isTargetFileAbailable(targetFilePath);
			if (statusNo == 200 || statusNo == 403)
			{
				ResponseStatus = statusNo;
				this->targetFilePath = targetFilePath;
				return ;

			}
		}
		ResponseStatus = statusNo;
		this->targetFilePath = targetFilePath;
		return ;
	}
	else
	{
		statusNo = isTargetFileAbailable(SerachFileAbsolutePath);
		ResponseStatus = statusNo;
		this->targetFilePath = SerachFileAbsolutePath;
		return ;
	}
}

size_t	Response::getContentLength()
{
	struct stat buf;
	stat(targetFilePath.c_str(), &buf);
	return buf.st_size;
}

size_t	Response::getErrorContentLength()
{
	struct stat buf;
	stat(errorFilePath.c_str(), &buf);
	return buf.st_size;
}

int	Response::getTargetFileFd()
{
	int fd;
	fd = open(targetFilePath.c_str(), O_RDONLY);
	return fd;
}

int Response::getErrorFileFd()
{
	int fd;
	fd = open(errorFilePath.c_str(), O_RDONLY);
	return fd;
}

void Response::setResponseLine()
{

	std::map<int, std::string> ResponseMap;
	setResponseMap(ResponseMap);
	responseMessege.append(std::string("HTTP/1.1") + " ");
	responseMessege.append(ft_itos(ResponseStatus) + " " + ResponseMap[ResponseStatus] + "\n");
	responseMessege.append("Server: nginx/1.14.0 (Ubuntu)\n");
}

void Response::setDate()
{
	time_t timer;
	struct tm *gmt;
	time(&timer);
	gmt = gmtime(&timer);
	responseMessege.append("Date: ");
	responseMessege.append(GetDate()[gmt->tm_wday] + ", ");
	responseMessege.append(ft_itos(gmt->tm_mday) + " ");
	responseMessege.append(GetMonth()[gmt->tm_mon] + " ");
	responseMessege.append(ft_itos(gmt->tm_year + 1900) + " ");
	responseMessege.append(ft_itos(gmt->tm_hour) + ":" + ft_itos(gmt->tm_min) + ":" + ft_itos(gmt->tm_sec) + " ");
	responseMessege.append("GMT\n");
}

std::string Response::getErrorPage()
{
	errorFilePath = configServer.root + configServer.configCommon.errorPages[ft_itos(ResponseStatus)];
	return errorFilePath;
}

bool Response::isErrorFilePathExist()
{
	errorFilePath = configServer.root + configServer.configCommon.errorPages[ft_itos(ResponseStatus)];
	struct stat buf;

	if (stat(errorFilePath.c_str(), &buf) == 0)
	{
		return true;
	}
	else
	{
		errorFilePath = "";
		return false;
	}
}

void Response::setContenType(std::string FilePath)
{
	std::map<int, std::string> ContentTypeMap;
	setResponseMap(ContentTypeMap);
	int i = FilePath.size() - 1;
	int count = 0;
	while (i >= 0)
	{
		if (FilePath[i] == '.')
			break ;
		++count;
		--i;
	}
	std::string sub = FilePath.substr(i , count + 1);
	responseMessege.append("Content-Type: ");
	responseMessege.append(GetContentType(sub) + "\n");

}
