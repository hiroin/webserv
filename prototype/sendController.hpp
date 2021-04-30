#ifndef SENDCONTROLLER
#define SENDCONTROLLER
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
  void	setFd(int fd);
  void	setSendData(char *buf, size_t len);

	SendController(){};
	~SendController(){};
};

#endif