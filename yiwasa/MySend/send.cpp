#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <errno.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <sys/wait.h>
#include <fcntl.h>
#include <map>
#include "sendController.hpp"

int main(int argc, char **argv, char **envp)// send のロジックを作っていく
{
	/*
		index 0 は 5行
		index 1 は 4行
		~
		index 4 は 1行
		で擬似レスポンスメッセージを作る。
	*/

	std::string ResponseStr[5];
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5 - i; j++)
		{
			ResponseStr[i].append("aaaaaaaaa\n");
		}
		ResponseStr[i].append("Finish\n");
	}



	/*
		SendControllerオブジェクトを kye=value の形で mapに格納

		keyは
	*/
	std::map<int, SendController> sendControllers; //map で用意する

	for(int i = 0; i < 5; i++)
	{
		std::string fileName = "log" + std::to_string(i + 1);
		/* hkamiya
			linuxの場合、O_CREATフラグがないとファイルが生成されない
			O_CREATフラグを設定した場合、権限の設定が必要なので、第3引数にS_IRWXUを追加
		*/
		int fd = open(fileName.c_str(), O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
		if (fd == -1)
		{
			std::cout << "ファイルオープン失敗" << std::endl;
		}
		/* hkamiya
			ノンブロッキングに変更
		*/
		int r = fcntl(fd, F_SETFL, O_NONBLOCK);
		if (r == -1)
		{
			std::cout << "fcntl失敗" << std::endl;
		}
		sendControllers[i].Initialize(fd, const_cast<char*>(ResponseStr[i].c_str()), ResponseStr[i].size());
	}

	//ここからが送っていくロジック

	/*
		sendControllersの中身が空になるまでsendをし続ける
		送信終了したfd からcloseして、sendControllerから削除していく

	*/
	while(!sendControllers.empty())
	{
		std::vector<int> eraseKey;
		std::map<int, SendController>::iterator now = sendControllers.begin();
		std::map<int, SendController>::iterator last = sendControllers.end();
		while(now != last)
		{
			bool isFinish = now->second.SendMessage(10);
			if(isFinish)
			{
				close(now->second.GetFd()); //sendが完了したらfdを閉じる
				eraseKey.push_back(now->first);// sendcontrollerから削除すべきKeyを選択する。
			}
			++now;
			usleep(10000);
		}
		for(int i = 0; i < eraseKey.size(); i++)
		{
			sendControllers.erase(eraseKey[i]);
		}
	}
}
