#ifndef _SENDCONTROLLER_H_
#define _SENDCONTROLLER_H_
#include <iostream>
#include <unistd.h>
#include <cerrno>

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
	ssize_t	LeftLength;

	public:
	bool	SendMessage(ssize_t len);
	void	Initialize(int fd, char *buf, ssize_t len);
	int		GetFd();
  void	setFd(int fd);
  void	setSendData(char *buf, ssize_t len);

	SendController(){};
	~SendController(){};
};

#endif