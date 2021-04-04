#ifndef SEND
#define SEND
#include <iostream>
#include <unistd.h>

/*
	SendControllerオブジェクト

	＝＝＝＝＝＝メンバ変数＝＝＝＝＝＝
	ClientFd : クライアントを示す fd
	ResponseMessage : レスポンスメッセージ
	LeftLength : あとどのくらいの長さがメッセージが残っているか

	＝＝＝＝メソッド＝＝＝＝
	SendMessage() : send関数の呼び出し
	Initialize() : SendControllerの初期化用
	GetFd() : ClientFdのゲッター
*/

class SendController
{
	private:
	int		CliantFd;
	char*	ResponseMessage;
	size_t	LeftLength;

	public:
	bool	SendMessage(size_t len);
	void	Initialize(int fd, char *buf, size_t len);
	int		GetFd();

	SendController(){};
	~SendController(){};
};

void	SendController::Initialize(int fd, char*buf, size_t len)
{
	CliantFd = fd;
	ResponseMessage = buf;
	LeftLength = len;
}

bool		SendController::SendMessage(size_t len)
{
	if (LeftLength <= len)
	{
    /* hkamiya
      writeもsendも第3引数で与えた数値分だけ書き込めるとは限らない()ので、
      書き込めたバイト数分だけ、ResponseMessageを進めて
      LeftLengthを減らす必要があります。
      ただ…、試験をする方法が思いつかないです。
    */
		int r = write(CliantFd, ResponseMessage, LeftLength);
    if (r == -1)
      throw std::runtime_error("write error.");
    if (r != LeftLength)
    {
      ResponseMessage += r;
      LeftLength -= r;
      return false;
    }
		LeftLength = 0;
		return (true);
	}
	else
	{
		int r = write(CliantFd, ResponseMessage, len);
    if (r == -1)
      throw std::runtime_error("write error.");
		ResponseMessage += r;
		LeftLength -= r;
		return (false);
	}
}

int		SendController::GetFd()
{
	return (CliantFd);
}

#endif