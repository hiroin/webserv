#include "Response.hpp"
#include "Config.hpp"
#include "HTTPMessageParser.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

/*******************************************************/
/**********************Util Fanctions*******************/

int ft_pow(int n, int times)
{
	int ret = 1;
	for (int i = 0; i < times; i++)
	{
		ret *= n;
	}
	return (ret);
}

std::string ft_itos(int nu)
{
	std::string ret;
	if (nu == 0)
		ret = "0";
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
	if (nu == 0)
	{
		return ("0");
	}
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

std::string ft_ultos(unsigned long nu)
{
	if (nu == 0)
	{
		return ("0");
	}
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

std::string getFileExtention(std::string FilePath)
{
	int i = FilePath.size() - 1;
	int count = 0;
	while (i >= 0)
	{
		if (FilePath[i] == '.')
			break;
		++count;
		--i;
	}
	return (FilePath.substr(i + 1, count));
}

bool isEightAlphas(std::string::iterator &itr)
{
	int count = 0;
	while (std::isalpha(*itr)) //language-rangeを回収
	{
		itr++;
		count++;
	}
	if (!(1 <= count && count <= 8))
		return false;
	return true;
}

bool isMatchQvalue(std::string::iterator &itr)
{
	if (*itr == '0')
	{
		++itr;
		if (*itr != '.' && *itr != ' ' && *itr != ',' && *itr != '\0')
			return false;
		if (*itr == '.')
		{
			++itr;
			int count = 0;
			while (std::isdigit(*itr))
			{
				++itr;
				count++;
			}
			if (count > 3)
				return false;
		}
		return true;
	}					  //qValue まで確認してreturn;
	else if (*itr == '1') //1.以外は認めない
	{
		++itr;
		if (*itr != '.' && *itr != ' ' && *itr != ',' && *itr != '\0')
			return false;
		if (*itr == '.')
		{
			int count = 0;
			++itr;
			while (*itr == '0')
			{
				++itr;
				count++;
			}
			if (count > 3 || (std::isdigit(*itr)))
				return false;
		}
		return true;
	} //qValue まで確認してreturn;
	else
		return false;
}

bool isMatchOption(std::string::iterator &itr)
{
	if (*itr != 'q')
		return false;
	++itr;
	if (*itr != '=')
		return false;
	++itr;
	if (!isMatchQvalue(itr))
		return false;
	return true;
}

void skipBreak(std::string::iterator &itr)
{
	while (*itr == ',' || *itr == ' ')
	{
		++itr;
	}
}

bool isMatchLanguageRange(std::string::iterator &itr)
{
	if (!isEightAlphas(itr))
		return false;
	if (*itr == '-')
	{
		++itr;
		if (!isEightAlphas(itr))
			return false;
	}
	return true;
}

float ft_stof(std::string str)
{
	float ret = 0;
	std::string upperPoint;
	std::string underPoint;
	std::string::iterator itr = str.begin();
	while (*itr != '.')
	{
		ret *= 10;
		ret += *itr - '0';
		++itr;
	}
	++itr;
	int count = 0;
	while (std::isdigit(*itr))
	{
		ret *= 10;
		ret += *itr - '0';
		++itr;
		count++;
	}
	ret /= (float)ft_pow(10, count);
	return (ret);
}

void getAcceptLanguages(std::map<std::string, std::vector<std::string>> &AcceptLanguageMap, std::string::iterator &itr)
{
	std::string LanguageRange;
	std::string qValue = "1";
	while (std::isalpha(*itr) || *itr == '*')
	{
		LanguageRange.push_back(*itr);
		++itr;
	}
	if (*itr == '-')
	{
		LanguageRange.push_back(*itr);
		++itr;
		while (std::isalpha(*itr))
		{
			LanguageRange.push_back(*itr);
			++itr;
		}
	}
	if (*itr == ';')
	{
		qValue.clear();
		++itr;
		++itr;
		++itr;
		while (std::isdigit(*itr) || *itr == '.')
		{
			qValue.push_back(*itr);
			++itr;
		}
	}
	AcceptLanguageMap[qValue].push_back(LanguageRange);
	skipBreak(itr);
}

void setResponseMap(std::map<int, std::string> &ResponseMap)
{
	ResponseMap[100] = "Continue";
	ResponseMap[101] = "Switching Protocols";
	ResponseMap[103] = "Early Hints";
	ResponseMap[200] = "OK";
	ResponseMap[201] = "Created";
	ResponseMap[202] = "Accepted";
	ResponseMap[203] = "Non-Authoritative Information";
	ResponseMap[204] = "No Content";
	ResponseMap[205] = "Reset Content";
	ResponseMap[206] = "Partial Content";
	ResponseMap[300] = "Multiple Choices";
	ResponseMap[301] = "Moved Permanently";
	ResponseMap[302] = "Found";
	ResponseMap[303] = "See Other";
	ResponseMap[304] = "Not Modified";
	ResponseMap[307] = "Temporary Redirect";
	ResponseMap[308] = "Permanent Redirect";
	ResponseMap[400] = "Bad Request";
	ResponseMap[401] = "Unauthorized";
	ResponseMap[402] = "Payment Required";
	ResponseMap[403] = "Forbidden";
	ResponseMap[404] = "Not Found";
	ResponseMap[405] = "Method Not Allowed";
	ResponseMap[406] = "Not Acceptable";
	ResponseMap[407] = "Proxy Authentication Required";
	ResponseMap[408] = "Request Timeout";
	ResponseMap[409] = "Conflict";
	ResponseMap[410] = "Gone";
	ResponseMap[411] = "Length Required";
	ResponseMap[412] = "Precondition Failed";
	ResponseMap[413] = "Payload Too Large";
	ResponseMap[414] = "URI Too Long";
	ResponseMap[415] = "Unsupported Media Type";
	ResponseMap[416] = "Range Not Satisfiable";
	ResponseMap[417] = "Expectation Failed";
	ResponseMap[418] = "I'm a teapot";
	ResponseMap[422] = "Unprocessable Entity";
	ResponseMap[425] = "Too Early";
	ResponseMap[426] = "Upgrade Required";
	ResponseMap[428] = "Precondition Required";
	ResponseMap[429] = "Too Many Requests";
	ResponseMap[431] = "Request Header Fields Too Large";
	;
	ResponseMap[451] = "Unavailable For Legal Reasons";
	ResponseMap[500] = "Internal Server Error";
	ResponseMap[501] = "Not Implemented";
	ResponseMap[502] = "Bad Gateway";
	ResponseMap[503] = "Service Unavailable";
	ResponseMap[504] = "Gateway Timeout";
	ResponseMap[505] = "HTTP Version Not Supported";
	ResponseMap[506] = "Variant Also Negotiates";
	ResponseMap[507] = "Insufficient Storage";
	ResponseMap[508] = "Loop Detected";
	ResponseMap[510] = "Not Extended";
	ResponseMap[511] = "Network Authentication";
}

void getAcceptCharset(std::map<std::string, std::vector<std::string>> &AcceptCharsetMap, std::string::iterator &itr)
{
	std::string Charset;
	std::string qValue = "1";
	while (std::isdigit(*itr) || std::isalpha(*itr) || (*itr == '!') || (*itr == '#') || (*itr == '$') || (*itr == '%') || (*itr == '&') || (*itr == '\'') || (*itr == '*') || (*itr == '+') || (*itr == '-') || (*itr == '.') || (*itr == '^') || (*itr == '_') || (*itr == '`') || (*itr == '|') || (*itr == '~'))
	{
		Charset.push_back(*itr);
		++itr;
	}
	if (*itr == ';')
	{
		qValue.clear();
		++itr;
		++itr;
		++itr;
		while (std::isdigit(*itr) || *itr == '.')
		{
			qValue.push_back(*itr);
			++itr;
		}
	}
	AcceptCharsetMap[qValue].push_back(Charset);
	skipBreak(itr);
}

bool isMatchCharset(std::string::iterator &itr)
{
	while (std::isdigit(*itr) || std::isalpha(*itr) || (*itr == '!') || (*itr == '#') || (*itr == '$') || (*itr == '%') || (*itr == '&') || (*itr == '\'') || (*itr == '*') || (*itr == '+') || (*itr == '-') || (*itr == '.') || (*itr == '^') || (*itr == '_') || (*itr == '`') || (*itr == '|') || (*itr == '~'))
	{
		++itr;
	}
	if (!(std::isprint(*itr)) && *itr != '\0')
		return false; //非表示文字が入ってたらreturn
	return true;
}

int isTheFileExist(std::string targetFile)
{
	int fd = open(targetFile.c_str(), O_RDONLY);
	if (fd != -1) //ファイルが存在して検索できた
	{
		close(fd);
		return (200);
	}
	else //失敗したから、errnoチェックして確認
	{
		switch (errno)
		{
		case EACCES:
			return (403);
		default:
			return (404);
		}
	}
}

void removeFilePart(std::string &SerachFileAbsolutePath)
{
	std::string::reverse_iterator first = SerachFileAbsolutePath.rbegin();
	std::string::reverse_iterator last = SerachFileAbsolutePath.rend();
	int count = 0;
	while (first != last)
	{
		if (*first == '/')
			break;
		++first;
		count++;
	}
	size_t size = SerachFileAbsolutePath.size();
	SerachFileAbsolutePath = SerachFileAbsolutePath.substr(0, size - count);
}

std::string getDatetimeStr()
{
	time_t t = time(NULL);
	const tm *localTime = localtime(&t);
	std::stringstream s;
	s << "20" << localTime->tm_year - 100;
	// setw(),setfill()で0詰め
	s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
	s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
	s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
	s << std::setw(2) << std::setfill('0') << localTime->tm_min;
	s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
	// std::stringにして値を返す
	return s.str();
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/**********************Util Fanctions*******************/
/*******************************************************/

/*******************************************************/
/**********************Member Functions*******************/

bool Response::isMatchAcceptLanguageFromat(std::string src)
{
	std::string::iterator itr = src.begin();
	std::string::iterator last = src.end();
	while (itr != last)
	{
		if (*itr == '*')
		{
			++itr;
		}
		else if (!isMatchLanguageRange(itr))
			return false;
		//ここまでで、langeage-rangeが回収できたとする
		//次は、オプションの有無を確かめる
		if (*itr == ';') //オプションのチェック
		{
			++itr;
			if (!isMatchOption(itr))
				return (false);
		}
		//オプションまで見たので、ブレイクを取り除く処理
		skipBreak(itr);
	}
	return (true);
}

std::map<std::string, std::vector<std::string>> Response::parseAcceptLanguage(std::string src)
{
	//ここに入ってくる時点で、Accept-Languageの形式は満たしていることが決定している
	std::map<std::string, std::vector<std::string>> AcceptLanguageMap;
	std::string::iterator itr = src.begin();
	std::string::iterator last = src.end();
	while (itr != last)
	{
		getAcceptLanguages(AcceptLanguageMap, itr);
	}
	AcceptLanguageMap[std::string("-")].push_back(std::string("*"));
	return AcceptLanguageMap;
}

s_ConfigCommon Response::getConfigCommon()
{
	if (configLocation.path.size() != 0) //Locationがある時
	{
		return (configLocation.configCommon);
	}
	else
	{
		return (configServer.configCommon);
	}
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

std::string Response::GetContentType(std::string key) //この拡張子リストになかったらapplication/octet-stream を返す。
{
	std::map<std::string, std::string> TypeMap;

	TypeMap["aac"] = "audio/aac";																   //AAC 音声
	TypeMap["abw"] = "application/x-abiword";													   //AbiWord文書
	TypeMap["arc"] = "application/x-freearc";													   //(複数のファイルが埋め込まれた) アーカイブ文書
	TypeMap["avi"] = "video/x-msvideo";															   //AVI: Audio Video Interleave
	TypeMap["azw"] = "application/vnd.amazon.ebook";											   //Amazon Kindle eBook 形式
	TypeMap["bin"] = "application/octet-stream";												   //任意の種類のバイナリーデータ
	TypeMap["bmp"] = "image/bmp";																   //Windows OS/2 ビットマップ画像
	TypeMap["bz"] = "application/x-bzip";														   //BZip アーカイブ
	TypeMap["bz2"] = "application/x-bzip2";														   //BZip2 アーカイブ
	TypeMap["csh"] = "application/x-csh";														   //C-Shell スクリプト
	TypeMap["css"] = "text/css";																   //カスケーディングスタイルシート (CSS)
	TypeMap["csv"] = "text/csv";																   //カンマ区切り値 (CSV)
	TypeMap["doc"] = "application/msword";														   //Microsoft Word
	TypeMap["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";   // Microsoft Word (OpenXML)
	TypeMap["eot"] = "application/vnd.ms-fontobject";											   //MS 埋め込み OpenType フォント
	TypeMap["epub"] = "application/epub+zip";													   //電子出版 (EPUB)
	TypeMap["gz"] = "application/gzip";															   //GZip 圧縮アーカイブ
	TypeMap["gif"] = "image/gif";																   //グラフィック交換形式 (GIF)
	TypeMap["htm"] = "text/html";																   //ハイパーテキストマークアップ言語 (HTML)
	TypeMap["html"] = "text/html";																   //ハイパーテキストマークアップ言語 (HTML)
	TypeMap["ico"] = "image/vnd.microsoft.icon";												   //アイコン形式
	TypeMap["ics"] = "text/calendar";															   //iCalendar 形式
	TypeMap["jar"] = "Java Archive (JAR)";														   //application/java-archive
	TypeMap["jpeg"] = "image/jpeg";																   //JPEG 画像
	TypeMap["jpg"] = "image/jpeg";																   //JPEG 画像
	TypeMap["js"] = "text/javascript";															   //JavaScript
	TypeMap["json"] = "application/json";														   //JSON 形式
	TypeMap["jsonld"] = "application/ld+json";													   //JSON-LD 形式
	TypeMap["midi"] = "audio/x-midi";															   //Musical Instrument Digital Interface (MIDI)
	TypeMap["mid"] = "audio/midi";																   //Musical Instrument Digital Interface (MIDI)
	TypeMap["mjs"] = "text/javascript";															   //JavaScript モジュール
	TypeMap["mp3"] = "audio/mpeg";																   //MP3 音声
	TypeMap["mpeg"] = "video/mpeg";																   //MPEG 動画
	TypeMap["mpkg"] = "application/vnd.apple.installer+xml";									   //Apple Installer Package
	TypeMap["odp"] = "application/vnd.oasis.opendocument.presentation";							   //OpenDocuemnt プレゼンテーション文書
	TypeMap["ods"] = "application/vnd.oasis.opendocument.spreadsheet";							   //OpenDocuemnt 表計算文書
	TypeMap["odt"] = "application/vnd.oasis.opendocument.text";									   //OpenDocument テキスト文書
	TypeMap["oga"] = "audio/ogg";																   //OGG 音声
	TypeMap["ogv"] = "video/ogg";																   //OGG 動画
	TypeMap["ogx"] = "application/ogg";															   //OGG
	TypeMap["opus"] = "audio/opus";																   //Opus 音声
	TypeMap["otf"] = "font/otf";																   //OpenType フォント
	TypeMap["png"] = "image/png";																   //Portable Network Graphics
	TypeMap["pdf"] = "application/pdf";															   //Adobe Portable Document Format (PDF)
	TypeMap["php"] = "application/x-httpd-php";													   //Hypertext Preprocessor (Personal Home Page)
	TypeMap["ppt"] = "application/vnd.ms-powerpoint";											   //Microsoft PowerPoint
	TypeMap["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation"; //     Microsoft PowerPoint (OpenXML)
	TypeMap["rar"] = "application/vnd.rar";														   //RAR アーカイブ
	TypeMap["rtf"] = "application/rtf";															   //リッチテキスト形式 (RTF)
	TypeMap["sh"] = "application/x-sh";															   //Bourne shell スクリプト
	TypeMap["svg"] = "image/svg+xml";															   //Scalable Vector Graphics (SVG)
	TypeMap["swf"] = "application/x-shockwave-flash";											   //Small web format (SWF) または Adobe Flash 文書
	TypeMap["tar"] = "application/x-tar";														   //Tape Archive (TAR)
	TypeMap["tif"] = "image/tiff";																   //Tagged Image File Format (TIFF)
	TypeMap["tiff"] = "image/tiff";																   //Tagged Image File Format (TIFF)
	TypeMap["ts"] = "video/mp2t";																   //MPEG transport stream
	TypeMap["ttf"] = "font/ttf";																   //TrueType フォント
	TypeMap["txt"] = "text/plain";																   //テキストファイル (一般に ASCII or ISO 8859-<em>n</em>)
	TypeMap["vsd"] = "application/vnd.visio";													   //Microsoft Visio
	TypeMap["wav"] = "audio/wav";																   //Waveform 音声形式
	TypeMap["weba"] = "audio/webm";																   //WEBM 音声
	TypeMap["webm"] = "video/webm";																   //WEBM 動画
	TypeMap["webp"] = "image/webp";																   //WEBP 画像
	TypeMap["woff"] = "font/woff";																   //Web Open Font Format (WOFF)
	TypeMap["woff2"] = "font/woff2";															   //Web Open Font Format (WOFF)
	TypeMap["xhtml"] = "application/xhtml+xml";													   //XHTML
	TypeMap["xls"] = "application/vnd.ms-excel";												   //Microsoft Excel
	TypeMap["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	TypeMap["xml"] = "application/xml";					//XML (一般のユーザから読める場合)
	TypeMap["xml"] = "text/xml";						//XML (一般のユーザから読めない場合)
	TypeMap["xul"] = "application/vnd.mozilla.xul+xml"; //XUL
	TypeMap["zip"] = "application/zip";					//ZIP アーカイブ
	TypeMap["3gp"] = "video/3gpp";						//3GPP 音声/動画コンテナー
	TypeMap["3gp"] = "audio/3gpp";						//3GPP 音声/動画コンテナー (動画含まず)
	TypeMap["3g2"] = "video/3gpp2";						//3GPP2 音声/動画コンテナー
	TypeMap["3g2"] = "audio/3gpp2";						//3GPP2 音声/動画コンテナー (動画含まず)
	TypeMap["7z"] = "application/x-7z-compressed";		//7-zipアーカイブ

	return (TypeMap[key]);
}

Response::~Response(){};

std::string Base64Encode(std::string szStr)
{
	std::string szB64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	char *szEnc;
	int iLen, i, j;
	iLen = szStr.size();
	szEnc = new char[(int)((float)iLen * 1.5 + 4)];

	j = 0;
	for (i = 0; i < (iLen - (iLen % 3)); i += 3)
	{
		szEnc[j] = szB64[(szStr[i] & 0xfc) >> 2];
		szEnc[j + 1] = szB64[((szStr[i] & 0x03) << 4) | ((szStr[i + 1] & 0xf0) >> 4)];
		szEnc[j + 2] = szB64[((szStr[i + 1] & 0x0f) << 2) | ((szStr[i + 2] & 0xc0) >> 6)];
		szEnc[j + 3] = szB64[(szStr[i + 2] & 0x3f)];
		j += 4;
	}
	i = iLen - (iLen % 3);
	switch (iLen % 3)
	{
	case 2:
	{
		szEnc[j] = szB64[(szStr[i] & 0xfc) >> 2];
		szEnc[j + 1] = szB64[((szStr[i] & 0x03) << 4) | ((szStr[i + 1] & 0xf0) >> 4)];
		szEnc[j + 2] = szB64[(szStr[i + 1] & 0x0f) << 2];
		szEnc[j + 3] = szB64[64];
		break;
	}
	case 1:
	{
		szEnc[j] = szB64[(szStr[i] & 0xfc) >> 2];
		szEnc[j + 1] = szB64[(szStr[i] & 0x03) << 4];
		szEnc[j + 2] = szB64[64];
		szEnc[j + 3] = szB64[64];
	}
	default:
		break;
	}
	szEnc[j + 4] = '\0';
	std::string ret = std::string(szEnc);
	delete[] szEnc;
	return (ret);
}

bool Response::isNecesarryAuth()
{
	s_ConfigCommon configCommon = getConfigCommon();
	if (configCommon.authBasicUid.size() == 0 && configCommon.authBasicPassword.size() == 0) //認証情報ない。
		return false;
	return true;
}

std::string Response::getEncodedServerCredential()
{
	s_ConfigCommon configCommon = getConfigCommon();
	std::string ServerCredential = configCommon.authBasicUid + ":" + configCommon.authBasicPassword;
	return (Base64Encode(ServerCredential));
}

bool Response::isRequestMatchAuth()
{
	std::string Authorization = client.hmp.headers_[std::string("authorization")];
	std::string Type;
	std::string credential;
	std::string EncodedServerCredential;

	size_t i = 0;
	for (i = 0; i < Authorization.size(); i++)
	{
		if (Authorization[i] == ' ')
		{
			Type = Authorization.substr(0, i);
			++i;
			break;
		}
	}
	if (Type != std::string("Basic"))
		return false;
	while (i < Authorization.size())
	{
		credential.push_back(Authorization[i]);
		++i;
	}
	EncodedServerCredential = getEncodedServerCredential();
	if (credential != EncodedServerCredential)
		return false;
	return true;
}

bool Response::isAuthorized()
{
	//Configを調べて、求めるリソースに認証が必要かをチェック
	if (isNecesarryAuth())
	{
		if (isRequestMatchAuth())
			return true;
		return false;
	}
	return true;
}

void Response::setWWWAuthenticate()
{
	responseMessege.append("WWW-Authenticate: ");
	responseMessege.append(std::string("basic "));
	responseMessege.append(std::string("realm=") + "\"" + getConfigCommon().authBasicRealm + "\"");
	responseMessege.append(std::string("\r\n"));
}

bool Response::isAcceptCharsetSet()
{
	return (client.hmp.headers_[std::string("accept-charset")].size() != 0);
}

bool Response::isMatchAcceptCharsetFromat(std::string src)
{
	std::string::iterator itr = src.begin();
	std::string::iterator last = src.end();
	while (itr != last)
	{
		if (!isMatchCharset(itr))
			return false;
		//ここまでで、langeage-rangeが回収できたとする
		//次は、オプションの有無を確かめる
		if (*itr == ';') //オプションのチェック
		{
			++itr;
			if (!isMatchOption(itr))
				return (false);
		}
		//オプションまで見たので、ブレイクを取り除く処理
		skipBreak(itr);
	}
	return (true);
}

std::map<std::string, std::vector<std::string>> Response::parseAcceptCharset(std::string src)
{
	//ここに入ってくる時点で、Accept-Languageの形式は満たしていることが決定している
	std::map<std::string, std::vector<std::string>> AcceptLanguageMap;
	std::string::iterator itr = src.begin();
	std::string::iterator last = src.end();
	while (itr != last)
	{
		getAcceptCharset(AcceptLanguageMap, itr);
	}
	AcceptLanguageMap[std::string("-")].push_back(std::string("*"));
	return AcceptLanguageMap;
}

bool Response::isTransferEncodingChunked()
{
	return (client.hmp.headers_["transfer-encoding"] == std::string("chunked"));
}

bool Response::isContentLength()
{
	return (client.hmp.headers_["content-length"].size() != 0);
}

bool isExtention(std::string absolutePath)
{
	if (absolutePath.size() == 0)
		return false;
	if (absolutePath[absolutePath.size() - 1] == '/')
		return true;
	std::string::reverse_iterator first = absolutePath.rbegin();
	std::string::reverse_iterator last = absolutePath.rend();
	while (first != last)
	{
		if (*first == '.') //拡張子あったらtrue
			return true;
		if (*first == '/') //次に "/" が出てくるまで見にいく
			break;
		++first;
	}
	return false;
}

void Response::addSlashOnAbsolutePath()
{
	if (!isExtention(client.hmp.absolutePath_))
	{
		client.hmp.absolutePath_ += std::string("/");
	}
}

bool Response::isReadable(std::string filePath)
{
	int ret = open(filePath.c_str(), O_RDONLY);
	if (ret == -1)
		return false;
	close(ret);
	return true;
}

bool Response::isExecutable(std::string filePath)
{
	struct stat buf;
	int ret = stat(filePath.c_str(), &buf);
	if (ret == -1)
		return false;
	if (buf.st_mode & S_IXUSR)
	{
		return true;
	}
	return false;
}

bool Response::isCgiFile()
{
	s_ConfigCommon configCommon = getConfigCommon();
	std::vector<std::string> cgiScripts = configCommon.cgiScripts;
	std::string fileExtention = "." + getFileExtention(targetFilePath);
	for (size_t i = 0; i < cgiScripts.size(); i++)
	{
		if (cgiScripts[i] == fileExtention)
			return true;
	}
	return false;
}

Response::Response(Client &client, Config &config) : ResponseStatus(-1), config(config), client(client), isAutoIndexApply(false), isCGI(false), readFd(-1), writeFd(-1)
{
	addSlashOnAbsolutePath();
	DecideConfigServer();	//使用するserverディレクティブを決定
	DecideConfigLocation(); //使用するlocationディレクティブを決定
	/*Authorization をチェック*/
	if (!isAuthorized()) //認証情報に問題があったら
	{
		ResponseStatus = 401;
		setResponseLine(); //responseStatus と serverNameヘッダを設定
		setDate();
		setWWWAuthenticate();
		responseMessege.append(std::string("Content-Length: 0\r\n"));
		responseMessege.append(std::string("\r\n"));
		client.status = SEND;
		return;
	}
	/* メソッドが許可されているかを判断 */
	if (isMethodAllowed())
	{
		//ここから、メソッド毎に処理を分けて書いていく
		if (client.hmp.method_ == httpMessageParser::GET || client.hmp.method_ == httpMessageParser::HEAD)
		{
			if (isAcceptCharsetSet())
			{
				std::string AcceptCharsetValue = client.hmp.headers_[std::string("accept-charset")];
				if (isMatchAcceptCharsetFromat(AcceptCharsetValue))
				{
					AcceptCharsetMap = parseAcceptCharset(AcceptCharsetValue);
				}
				else
					ResponseStatus = 406;
			}
			if (isAcceptLanguageSet())
			{
				std::string AcceptLanguageValue = client.hmp.headers_[std::string("accept-language")];
				if (isMatchAcceptLanguageFromat(AcceptLanguageValue))
				{
					AcceptLanguageMap = parseAcceptLanguage(AcceptLanguageValue);
				}
				else
					ResponseStatus = 406;
			}
			setTargetFileAndStatus(); //探しにいくファイルパスと、レスポンスステータスを決定
			if (ResponseStatus == 200)
			{
				//cgi を実行か判断
				if (isCgiFile())
				{
					if (getFileExtention(targetFilePath) == std::string("php"))
					{
						if (execPhpCgi())
							isCGI = true;
					}
					else if (getFileExtention(targetFilePath) == std::string("bla"))
					{
						if (execCgiTester())
							isCGI = true;
					}
					else
					{
						if (execCgi())
							isCGI = true;
						//phpでないCGI の実行
					}
				}
			}
		}
		else if (client.hmp.method_ == httpMessageParser::DELETE)
		{
			s_ConfigCommon configCommon = getConfigCommon();
			std::string SearchAbsolutePath = GetSerachAbsolutePath();
			if (isDirectoryAvailable())
			{
				if (SearchAbsolutePath[SearchAbsolutePath.size() - 1] != '/') //directory がOKだったらこの下で書き込み
				{
					//指定されたリソース削除できるかできるかをチェック
					this->targetFilePath = SearchAbsolutePath;
					int fd = open(this->targetFilePath.c_str(), O_RDWR);
					if (fd == -1)
					{
						if (errno == EACCES)
							ResponseStatus = 403;
						else
							ResponseStatus = 404;
					}
					else
					{
						close(fd);
						int ret = unlink(targetFilePath.c_str());
						if (ret == -1)
							ResponseStatus = 500;
						else
							ResponseStatus = 204;
					}
				}
				else
				{
					ResponseStatus = 404; //絶対パスが"/"で終わってたら404
				}
			}
			else
			{
				ResponseStatus = 403;
			}
		}
		else if (client.hmp.method_ == httpMessageParser::PUT || client.hmp.method_ == httpMessageParser::POST)
		{
			if (isTransferEncodingChunked() || isContentLength())
			{
				s_ConfigCommon configCommon = getConfigCommon();
				int clientMaxBodySize = configCommon.clientMaxBodySize;
				if (clientMaxBodySize < 0)
					clientMaxBodySize = 0;
				if (client.body.size() > static_cast<unsigned int>(clientMaxBodySize))
				{
					ResponseStatus = 413;
				}
				else
				{
					std::string SearchAbsolutePath = GetSerachAbsolutePath();
					PutPostBody = client.hmp.body_;
					if (client.hmp.method_ == httpMessageParser::PUT)
					{
						if (isDirectoryAvailable())
						{
							if (SearchAbsolutePath[SearchAbsolutePath.size() - 1] != '/') //directory がOKだったらこの下で書き込み
							{
								//指定されたリソースが作成できるかをチェック
								this->targetFilePath = SearchAbsolutePath;
								int isExist = isTheFileExist(this->targetFilePath);
								int fd = open(this->targetFilePath.c_str(), O_RDWR | O_CREAT, S_IRWXU);
								if (fd == -1)
								{
									ResponseStatus = 403;
									return;
								}
								close(fd);
								if (isExist == 200)
									ResponseStatus = 204;
								else
									ResponseStatus = 201;
							}
						}
						else
							ResponseStatus = 403;
					}
					else //CGI にリクエストをだす
					{
						//absolutePath の最後が'/' で終了していたらファイル作って、読み込み用のFDを返す
						if (client.hmp.absolutePath_[client.hmp.absolutePath_.size() - 1] == '/')
						{
							//uploadPath にタイムスタンプでファイルを作成しておく
							std::string createFilePath = getConfigCommon().uploadPath + "/" + getDatetimeStr();
							int fd = open(createFilePath.c_str(), O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
							if (fd == -1)
							{
								ResponseStatus = 403;
							}
							else
							{
								close(fd);
								targetFilePath = createFilePath;
								ResponseStatus = 201;
							}
						}
					}
				}
			}
			else
			{
				ResponseStatus = 403;
			}
		}
	}
	setResponseLine(); //responseStatus と serverNameヘッダを設定
	setDate();
	if (ResponseStatus != 200 && ResponseStatus != 201 && ResponseStatus != 204)
	{
		if (ResponseStatus == 405)
			setAllow();
		//erro_pageを探して、あったらContent-Type,Content-Lengthを記入して
		if (isErrorFilePathExist())
		{
			setContenType();
			setContentLength();
			if (client.hmp.method_ == httpMessageParser::HEAD)
			{
				client.status = SEND;
			}
			else
				client.status = READ;
			return;
		}
		else
		{
			responseMessege.append(std::string("Content-Length: 5\r\n"));
			responseMessege.append(std::string("\r\n"));
			responseMessege.append(std::string("Error"));
			client.status = SEND;
			return;
		}
	}
	if (isAutoIndexApply)
	{
		makeAutoIndexResponse();
		client.status = SEND;
		return;
	}
	if (client.hmp.method_ == httpMessageParser::HEAD)
	{
		setContenType(); //Languageを考えて選択する。
		setLastModified();
		setContentLength();
		if (isLanguageFile(targetFilePath, getFileExtention(targetFilePath)))
			setContentLanguage();
		client.status = SEND;
	}
	if (client.hmp.method_ == httpMessageParser::GET)
	{
		if (!isCGI)
		{
			setContenType();
			setContentLength();
			if (isLanguageFile(targetFilePath, getFileExtention(targetFilePath)))
				setContentLanguage();
		}
		client.status = READ;
	}
	if (client.hmp.method_ == httpMessageParser::PUT)
	{
		setLocation();
		responseMessege.append(std::string("Content-Length: 0\r\n\r\n"));
		client.status = WRITE;
	}
	if (client.hmp.method_ == httpMessageParser::POST)
	{
		responseMessege.append(std::string("Content-Length: 0\r\n\r\n"));
		client.status = WRITE;
	}
	if (client.hmp.method_ == httpMessageParser::DELETE)
	{
		responseMessege.append(std::string("Content-Length: 0\r\n\r\n"));
		client.status = SEND;
	}
}

Response::Response(int ErrorCode, Client &client, Config &config) : ResponseStatus(-1), config(config), client(client), readFd(-1), writeFd(-1)
{
	addSlashOnAbsolutePath();
	DecideConfigServer();	//使用するserverディレクティブを決定
	DecideConfigLocation(); //使用するlocationディレクティブを決定
	ResponseStatus = ErrorCode;
	setResponseLine(); //responseStatus と serverNameヘッダを設定
	setDate();		   //Dateヘッダを設定
	if (isErrorFilePathExist())
	{
		setContenType();
		setContentLength();
		client.status = READ;
		return;
	}
	responseMessege.append("Content-Length: 0\r\n");
	client.status = SEND;
}

int Response::getFileFdForWrite()
{
	if (writeFd == -1)
	{
		writeFd = open(targetFilePath.c_str(), O_RDWR | O_CREAT, S_IRWXU);
		if (writeFd == -1)
			return (-1);
	}
	return (writeFd);
}

bool Response::isAcceptLanguageSet()
{
	return (client.hmp.headers_[std::string("accept-language")].size() != 0);
}

bool Response::isMethodAllowed()
{
	bool *allowMethodsBool;
	s_ConfigCommon configCommon = getConfigCommon();
	allowMethodsBool = configCommon.allowMethodsBool;
	if (allowMethodsBool[client.hmp.method_])
	{
		return true;
	}
	else
	{
		ResponseStatus = 405;
		return false;
	}
}

void Response::setAllow()
{
	std::string AllowHeader = "Allow: ";
	s_ConfigCommon configCommon = getConfigCommon();
	std::vector<std::string> allowMethods = configCommon.allowMethods;
	for (size_t i = 0; i < allowMethods.size(); i++)
	{
		if (i == allowMethods.size() - 1)
		{
			AllowHeader.append(allowMethods[i] + "\r\n");
		}
		else
		{
			AllowHeader.append(allowMethods[i] + ",");
		}
	}
	responseMessege.append(AllowHeader);
}

bool Response::DecideConfigServer()
{
	int clientPort = client.port;
	std::string clientHostName = client.host;
	HTTPMessageParser hmp = client.hmp;

	//ここから、configをなめていって該当設定を見つけよう。
	std::vector<s_ConfigServer> servers = config.configGlobal.servers;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].port == clientPort && servers[i].host == clientHostName) //クライアントとportが一致する設定は必ずある
		{
			std::vector<std::string> serverNames = servers[i].serverNames;
			std::string requestHOST = hmp.headers_["host"];
			for (size_t j = 0; j < serverNames.size(); j++)
			{
				if (serverNames[j] == requestHOST)
				{
					this->configServer = servers[i];
					return (true);
				}
			}
		}
	}
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].port == clientPort && servers[i].host == clientHostName) // serverNameで一致するものがなかったら一番上のもの
		{
			this->configServer = servers[i];
			return (true);
		}
	}
	return (true);
}

bool Response::DecideConfigLocation()
{
	std::string AbsolutePath = this->client.hmp.absolutePath_;
	std::vector<s_ConfigLocation> locations = this->configServer.locations;

	for (size_t i = 0; i < locations.size(); i++) // 文字が長い順番でlocationが入ってる。
	{
		if (AbsolutePath.find(locations[i].path) == 0) //location path と absolutePathが先頭一致した場合
		{
			configLocation = locations[i];
			return true;
		}
	}
	return false;
}

std::string Response::GetSerachAbsolutePath()
{
	std::string SerachAbsolutePath = "";
	std::string AbsolutePath = client.hmp.absolutePath_;
	if (configLocation.path.size() == 0 || configLocation.alias.size() == 0) //該当するLocationがなかった or aliasがなかった場合、rootが先頭につく
	{
		SerachAbsolutePath = configServer.root + AbsolutePath; //targetFilePath
	}
	else //locationがあれば、そのalisaを置換。
	{
		SerachAbsolutePath = AbsolutePath.replace(0, configLocation.path.size(), configLocation.alias);
	}
	return (SerachAbsolutePath);
}

//先にc

int Response::isLanguageFileExist(std::string SerachFileAbsolutePath)
{
	std::map<std::string, std::vector<std::string>>::reverse_iterator first = AcceptLanguageMap.rbegin();
	std::map<std::string, std::vector<std::string>>::reverse_iterator last = AcceptLanguageMap.rend();
	while (first != last)
	{
		int statusNo;
		std::string targetFile;
		std::vector<std::string> Languages = first->second;
		for (size_t i = 0; i < Languages.size(); i++)
		{
			if (Languages[i] == "*")
				targetFile = SerachFileAbsolutePath;
			else
				targetFile = SerachFileAbsolutePath + "." + Languages[i];
			statusNo = isTheFileExist(targetFile);
			switch (statusNo)
			{
			case 200:
				this->targetFilePath = targetFile; //見つかったら、filePath変更してreturn
				return (statusNo);
				break;
			case 403: //Permittion Denied だったらreturn
				return (statusNo);
			default:
				continue;
				break;
			}
		}
		++first;
	}
	return (406); //見つからなかったら406
}

void Response::setContentLanguage()
{
	std::string ContentLanguage = "Content-Language: ";
	ContentLanguage.append(getFileExtention(targetFilePath) + "\r\n");
	responseMessege.append(ContentLanguage);
	return;
}

int Response::isCharsetFileExist(std::string SerachFileAbsolutePath)
{
	std::map<std::string, std::vector<std::string>>::reverse_iterator first = AcceptCharsetMap.rbegin();
	std::map<std::string, std::vector<std::string>>::reverse_iterator last = AcceptCharsetMap.rend();
	while (first != last)
	{
		int statusNo;
		std::string targetFile;
		std::vector<std::string> Charset = first->second;
		for (size_t i = 0; i < Charset.size(); i++)
		{
			if (Charset[i] == "*")
				targetFile = SerachFileAbsolutePath;
			else
				targetFile = SerachFileAbsolutePath + "." + Charset[i];
			statusNo = isTheFileExist(targetFile);
			switch (statusNo)
			{
			case 200:
				this->targetFilePath = targetFile; //見つかったら、filePath変更してreturn
				return (statusNo);
				break;
			case 403: //Permittion Denied だったらreturn
				return (statusNo);
			default:
				continue;
				break;
			}
		}
		++first;
	}
	return (406); //見つからなかったら406
}

int Response::isCharsetAndLanguageFileExist(std::string SerachFileAbsolutePath)
{
	std::map<std::string, std::vector<std::string>>::reverse_iterator Cfirst = AcceptCharsetMap.rbegin();
	std::map<std::string, std::vector<std::string>>::reverse_iterator Clast = AcceptCharsetMap.rend();

	while (Cfirst != Clast)
	{
		int statusNo;
		std::vector<std::string> Charset = Cfirst->second;
		std::string targetFileWithCharset;

		for (size_t i = 0; i < Charset.size(); i++)
		{
			targetFileWithCharset = SerachFileAbsolutePath + "." + Charset[i]; // Charsetつけた

			std::map<std::string, std::vector<std::string>>::reverse_iterator Lfirst = AcceptLanguageMap.rbegin();
			std::map<std::string, std::vector<std::string>>::reverse_iterator Llast = AcceptLanguageMap.rend();
			while (Lfirst != Llast)
			{
				std::vector<std::string> Languages = Lfirst->second;
				for (size_t i = 0; i < Languages.size(); i++)
				{
					std::string targetFile;
					if (Languages[i] == "*")
						targetFile = targetFileWithCharset;
					else
						targetFile = targetFileWithCharset + "." + Languages[i];
					statusNo = isTheFileExist(targetFile);
					switch (statusNo)
					{
					case 200:
						this->targetFilePath = targetFile; //見つかったら、filePath変更してreturn
						return (statusNo);
						break;
					case 403: //Permittion Denied だったらreturn
						return (statusNo);
					default:
						continue;
					}
				}
				++Lfirst;
			}
		}
		++Cfirst;
	}
	return (406); //見つからなかったら406
}

int Response::isTargetFileAbailable(std::string SerachFileAbsolutePath)
{
	/**
	 * Accept-Language を回していく、それで発見できなかったら406
	 * **/
	if (AcceptLanguageMap.size() != 0 && AcceptCharsetMap.size() == 0) //AcceptLanguageはあるけど、AcceptCharsetはない
	{
		return (isLanguageFileExist(SerachFileAbsolutePath));
	}
	else if (AcceptLanguageMap.size() == 0 && AcceptCharsetMap.size() != 0) //AcceptLanguageはないけど、AcceptCharsetはある
	{
		return (isCharsetFileExist(SerachFileAbsolutePath));
	}
	else if (AcceptLanguageMap.size() != 0 && AcceptCharsetMap.size() != 0) //AcceptLanguageもあるし、AcceptCharsetもある
	{
		return (isCharsetAndLanguageFileExist(SerachFileAbsolutePath));
	}
	this->targetFilePath = SerachFileAbsolutePath;
	return (isTheFileExist(SerachFileAbsolutePath));
}

void Response::setTargetFileAndStatus() //GetSerachAbsolutePath() が返してくる物をみて、ファイルがそもそも存在するかをチェック
{
	if (ResponseStatus == 406)
		return;
	std::string SerachFileAbsolutePath = GetSerachAbsolutePath();
	if (SerachFileAbsolutePath[SerachFileAbsolutePath.size() - 1] == '/')
	{
		std::vector<std::string> indexs;
		s_ConfigCommon configCommon = getConfigCommon();
		int statusNo = 403;
		indexs = configCommon.indexs;
		for (size_t i = 0; i < indexs.size(); i++)
		{
			this->targetFilePath = SerachFileAbsolutePath + indexs[i]; //indexファイルを見ていく
			statusNo = isTargetFileAbailable(this->targetFilePath);
			if (statusNo == 200 || statusNo == 403) //403 と 200が発生したらそのままreturn
			{
				ResponseStatus = statusNo;
				return;
			}
		}
		// indexディレクティブがなかったら403
		ResponseStatus = statusNo;					 //ここにくる場合は、404 not found になってる (autoindex の場合は別だけど)
		if (isAutoIndex() && isDirectoryAvailable()) //autoindex の時はここに入ってくる。
		{
			isAutoIndexApply = true;
			getAutoIndexContent(); //AutoIndex のBody を作る。
			ResponseStatus = 200;
			//autoindex の情報入れる
		}
		this->targetFilePath = "";
		return;
	}
	else
	{
		int statusNo = isTargetFileAbailable(SerachFileAbsolutePath);
		ResponseStatus = statusNo;
		return;
	}
}

bool Response::isDirectoryAvailable()
{
	std::string SerachFileAbsolutePath = GetSerachAbsolutePath();
	if (SerachFileAbsolutePath[SerachFileAbsolutePath.size() - 1] != '/')
		removeFilePart(SerachFileAbsolutePath);
	DIR *ret = opendir(SerachFileAbsolutePath.c_str());
	if (ret == NULL)
		return false;
	closedir(ret);
	return true;
}

bool Response::isDirectory(std::string name)
{
	std::string pathname = GetSerachAbsolutePath() + name;
	struct stat sb;
	stat(pathname.c_str(), &sb);
	if (S_ISDIR(sb.st_mode))
		return true;
	else
		return false;
}

std::vector<std::string> Response::getDirectoryContents()
{
	struct dirent *rdir;
	std::vector<std::string> directoryContents;
	std::string SerachFileAbsolutePath = GetSerachAbsolutePath();
	DIR *dir = opendir(SerachFileAbsolutePath.c_str());

	while ((rdir = readdir(dir)) != NULL)
	{
		std::string dateName = std::string(rdir->d_name);
		if (dateName == std::string("."))
			continue;
		if (isDirectory(dateName))
		{
			directoryContents.push_back(std::string(dateName) + "/");
		}
		else
		{
			directoryContents.push_back(std::string(dateName));
		}
	}
	closedir(dir);
	return (directoryContents);
}

std::string Response::makeATag(std::string dataName)
{
	std::string aTag;
	aTag.append(std::string("<a href="));
	aTag.append(std::string("\"" + dataName + "\">"));
	aTag.append(std::string(dataName));
	aTag.append(std::string("</a>"));
	if (dataName[dataName.size() - 1] == '/')
	{
		aTag.append(std::string("\r\n"));
		return aTag;
	}
	else
	{
		aTag.append(std::string("                                             "));
		struct stat buf;
		stat(dataName.c_str(), &buf);
		// std::cout << "=====================" << ctime(&(buf.st_ctime)) << "==============================" << std::endl;
		aTag.append(std::string(ctime(&(buf.st_ctime))) + "\r\n");
		return aTag;
	}
}

void Response::getAutoIndexContent()
{
	std::vector<std::string> directoryContents = getDirectoryContents();
	AutoIndexContent.append(std::string("<html>\r\n"));
	AutoIndexContent.append(std::string("<head><title>Index of "));
	AutoIndexContent.append(client.hmp.absolutePath_);
	AutoIndexContent.append(std::string("</title></head>\r\n"));
	AutoIndexContent.append(std::string("<body>\n"));
	AutoIndexContent.append(std::string("<h1>Index of "));
	AutoIndexContent.append(std::string(client.hmp.absolutePath_));
	AutoIndexContent.append(std::string("</h1><hr><pre>"));
	for (size_t i = 0; i < directoryContents.size(); i++)
	{
		std::string aTag;
		aTag = makeATag(directoryContents[i]);
		AutoIndexContent.append(aTag);
	}
	AutoIndexContent.append(std::string("</pre><hr></body>\r\n"));
	AutoIndexContent.append(std::string("</html>"));
}

void Response::makeAutoIndexResponse()
{
	std::string autoIndexSize = ft_itos(AutoIndexContent.size());
	responseMessege.append(std::string("Content-Type: text/html\r\n"));
	responseMessege.append(std::string("Content-Length: "));
	responseMessege.append(autoIndexSize + "\r\n");
	responseMessege.append(std::string("\r\n"));
	if (client.hmp.method_ == httpMessageParser::GET)
		responseMessege.append(AutoIndexContent);
}

bool Response::isAutoIndex()
{
	std::string SerachFileAbsolutePath = GetSerachAbsolutePath();
	return SerachFileAbsolutePath[SerachFileAbsolutePath.size() - 1] == '/' && getConfigCommon().autoindex == "on";
}

size_t Response::getContentLength()

{
	struct stat buf;
	stat(targetFilePath.c_str(), &buf);
	return buf.st_size;
}

int Response::getTargetFileFd()
{
	if (readFd == -1)
	{
		readFd = open(targetFilePath.c_str(), O_RDONLY);
		if (readFd == -1)
			return (-1);
	}
	return readFd;
}

void Response::setResponseLine()
{

	std::map<int, std::string> ResponseMap;
	setResponseMap(ResponseMap);
	responseMessege.append(std::string("HTTP/1.1") + " ");
	responseMessege.append(ft_itos(ResponseStatus) + " " + ResponseMap[ResponseStatus] + "\r\n");
	responseMessege.append("Server: nginx/1.14.0 (Ubuntu)\r\n");
}

void Response::setDate()
{
	time_t timer;
	struct tm *gmt;
	time(&timer);
	gmt = gmtime(&timer);
	responseMessege.append("Date: ");
	responseMessege.append(makeRFCDate(timer));
	responseMessege.append("\r\n");
}

std::string Response::makeRFCDate(time_t timestamp)
{
	struct tm *gmt;
	gmt = gmtime(&timestamp);
	std::string RFCDate;
	RFCDate.append(GetDate()[gmt->tm_wday] + ", ");
	RFCDate.append(ft_ito00(gmt->tm_mday) + " ");
	RFCDate.append(GetMonth()[gmt->tm_mon] + " ");
	RFCDate.append(ft_itos(gmt->tm_year + 1900) + " ");
	RFCDate.append(ft_ito00(gmt->tm_hour) + ":" + ft_ito00(gmt->tm_min) + ":" + ft_ito00(gmt->tm_sec) + " ");
	RFCDate.append("GMT");
	return RFCDate;
}

std::string Response::ft_ito00(int n)
{
	std::string ret;
	if (n <= 9)
	{
		ret += '0';
		ret += '0' + n;
		return ret;
	}
	while (n > 0)
	{
		char c[2];
		c[0] = '0' + n % 10;
		c[1] = '\0';
		ret.insert(0, std::string(c));
		n /= 10;
	}
	return (ret);
}

void Response::setLocation()
{
	responseMessege.append(std::string("Location: "));
	responseMessege.append(this->client.hmp.absolutePath_ + "\r\n");
}

bool Response::isErrorFilePathExist()
{
	s_ConfigCommon configCommon;
	if (configLocation.path.size() != 0) //Location優先
		configCommon = configLocation.configCommon;
	else
		configCommon = configServer.configCommon;
	std::string errorFilePath = configServer.root + configCommon.errorPages[ft_itos(ResponseStatus)];
	struct stat buf;

	if (stat(errorFilePath.c_str(), &buf) == 0 && !S_ISDIR(buf.st_mode))
	{
		targetFilePath = errorFilePath;
		return true;
	}
	else
	{
		targetFilePath = "";
		return false;
	}
}

bool Response::isLanguageFile(std::string FilePath, std::string fileExtention)
{
	static_cast<void>(FilePath);
	if (AcceptLanguageMap.size() == 0)
		return (false);

	//LanguageFile だったら、AcceptLanguageMapに該当する奴がいないかチェックしていって、あったらtrueを返す
	std::map<std::string, std::vector<std::string>>::reverse_iterator first = AcceptLanguageMap.rbegin();
	std::map<std::string, std::vector<std::string>>::reverse_iterator last = AcceptLanguageMap.rend();
	while (first != last)
	{
		std::vector<std::string> values = first->second;
		for (size_t i = 0; i < values.size(); i++)
		{
			if (values[i] == fileExtention)
			{
				return (true);
			}
		}
		++first;
	}
	return (false);
}

bool Response::isCharsetFile(std::string FilePath, std::string fileExtention)
{
	static_cast<void>(FilePath);
	if (AcceptCharsetMap.size() == 0)
		return (false);

	//LanguageFile だったら、AcceptLanguageMapに該当する奴がいないかチェックしていって、あったらtrueを返す
	std::map<std::string, std::vector<std::string>>::reverse_iterator first = AcceptCharsetMap.rbegin();
	std::map<std::string, std::vector<std::string>>::reverse_iterator last = AcceptCharsetMap.rend();
	while (first != last)
	{
		std::vector<std::string> values = first->second;
		for (size_t i = 0; i < values.size(); i++)
		{
			if (values[i] == fileExtention)
			{
				return (true);
			}
		}
		++first;
	}
	return (false);
}
std::string Response::removeLanguageAndCharsetFileExtention()
{
	std::string fileExtention = getFileExtention(targetFilePath);
	//このsubに該当する言語が入っているかを確認する。いたら切り取って、もう一個拡張子取りに行く

	if (isLanguageFile(targetFilePath, fileExtention))
	{
		std::string LanguageRemoved = targetFilePath.substr(0, targetFilePath.size() - fileExtention.size() - 1);
		fileExtention = getFileExtention(LanguageRemoved);
		if (isCharsetFile(LanguageRemoved, fileExtention))
		{
			std::string CharsetRemoved = LanguageRemoved.substr(0, LanguageRemoved.size() - fileExtention.size() - 1);
			fileExtention = getFileExtention(CharsetRemoved);
		}
	}
	else if (isCharsetFile(targetFilePath, fileExtention))
	{
		std::string CharsetRemoved = targetFilePath.substr(0, targetFilePath.size() - fileExtention.size() - 1);
		fileExtention = getFileExtention(CharsetRemoved);
	}
	return (fileExtention);
}

std::string Response::getCharsetExtention()
{
	std::string fileExtention = getFileExtention(targetFilePath);
	if (isLanguageFile(targetFilePath, fileExtention))
	{
		std::string LanguageRemoved = targetFilePath.substr(0, targetFilePath.size() - fileExtention.size() - 1);
		fileExtention = getFileExtention(LanguageRemoved);
		if (isCharsetFile(targetFilePath, fileExtention))
			return fileExtention;
		else
			return "";
	}
	else if (isCharsetFile(targetFilePath, fileExtention))
	{
		fileExtention = getFileExtention(targetFilePath);
		return fileExtention;
	}
	return ("");
}

void Response::setContenType()
{
	/**
	 * Languageつきのファイルの対応をする必要あり。
	 * AcceptLanguageMapに一致する拡張子があるかどうかチェックして、あったら切り取る
	 * **/
	std::map<int, std::string> ContentTypeMap;
	setResponseMap(ContentTypeMap);

	std::string charsetFileExtention = getCharsetExtention();

	std::string fileExtention = removeLanguageAndCharsetFileExtention();

	responseMessege.append("Content-Type: ");
	std::string ContentType = GetContentType(fileExtention);
	if (ContentType == "")
		responseMessege.append(std::string("application/octet-stream"));
	else
		responseMessege.append(ContentType);
	//AcceptCharset を考えようか
	if (charsetFileExtention != "")
	{
		responseMessege.append(std::string("; "));
		responseMessege.append(std::string("charset=") + charsetFileExtention);
	}
	responseMessege.append(std::string("\r\n"));
}

void Response::setContentLength()
{
	std::string ContentLength = "Content-Length: ";
	size_t contentLen = getContentLength();
	ContentLength.append(ft_ltos((long)contentLen) + "\r\n");
	responseMessege.append(ContentLength);
}

time_t Response::getLastModified()

{
	struct stat buf;
	stat(targetFilePath.c_str(), &buf);
	return buf.st_mtime;
}

void Response::setLastModified()
{
	std::string LastModified = "Last-Modified: ";
	time_t timestamp = getLastModified();
	LastModified.append(makeRFCDate(timestamp) + "\r\n");
	responseMessege.append(LastModified);
}

void Response::AppendBodyOnResponseMessage(std::string body)
{
	responseMessege.append(std::string("\r\n"));
	responseMessege.append(body);
}

int Response::getCgiFd()
{
	if (isCGI)
		return readFd;
	else
		return -1;
}

std::vector<std::string> splitByCRLF(std::string string)
{
	std::string separator = std::string("\r\n\r\n"); // 区切り文字
	size_t separator_length = separator.length();	 // 区切り文字の長さ
	std::vector<std::string> ret;
	if (separator_length == 0)
	{
		ret.push_back(string);
	}
	else
	{
		size_t offset = 0;
		while (1)
		{
			size_t pos = string.find(separator, offset);
			if (pos == std::string::npos)
			{
				ret.push_back(string.substr(offset));
				break;
			}
			ret.push_back(string.substr(offset, pos - offset));
			offset = pos + separator_length;
		}
	}
	return ret;
}

void Response::mergeCgiResult(std::string CgiResult)
{
	if (CgiResult.find("\r\n\r\n") == std::string::npos)
	{
		responseMessege.clear();
		ResponseStatus = 500;
		setResponseLine();
		setDate();
		if (isErrorFilePathExist())
		{
			setContenType();
			setContentLength();
			if (client.hmp.method_ == httpMessageParser::HEAD)
			{
				client.status = SEND;
			}
			else
				client.status = READ;
			return;
		}
		else
		{
			responseMessege.append(std::string("Content-Length: 5\r\n"));
			responseMessege.append(std::string("\r\n"));
			responseMessege.append(std::string("Error"));
			client.status = SEND;
			return;
		}
	}
	else
	{
		std::vector<std::string> HeaderBody = splitByCRLF(CgiResult);
		size_t contentLength = HeaderBody[1].size();
		responseMessege.append(std::string("Content-Length: ") + ft_ultos(contentLength) + "\r\n");
		responseMessege.append(CgiResult);
	}
	client.status = SEND;
}
