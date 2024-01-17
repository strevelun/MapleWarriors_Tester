#include "Client.h"

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

#pragma comment(lib, "winmm.lib") 
#pragma comment( lib, "ws2_32.lib")

#define MAX_CLIENT_SIZE			300

double accTime = 0.0;
std::wstring nickname = L"Test";
std::wstring roomName = L"Room";

void LobbyTest();

enum class eChoice
{
	None,
	LobbyTest,
	ChoiceMax
};
const char* pServerIP = "192.168.219.107";
// const char * pServerIP = "220.127.242.178";
const int port = 30001;

int main()
{
	srand((unsigned int)time(nullptr));

	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;

	int clientCount = 0;
	int input;
	while (1)
	{
		system("cls");
		std::cout << "선택(로비테스트 : 1) : ";
		std::cin >> input;		
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(1024, '\n');
			continue;
		}
		if (input <= (int)eChoice::None || input >= (int)eChoice::ChoiceMax)
			continue;


		switch ((eChoice)input)
		{
		case eChoice::LobbyTest:
			LobbyTest();
			break;
		}
	}

	return 0;
}

void LobbyTest()
{
	// 랜덤한 시간에 랜덤한 숫자의 클라이언트 접속 or 로그아웃
	double totalAccTime = 0.0;
	std::vector<Client*>			vecClient;
	int clientID = 0;

	DWORD prevTime = timeGetTime();

	int loginTime = (rand() % 4) + 1;
	int logoutTime = (rand() % 10) + 1;
	//int logoutTime = 9999;

	int howMany, curClient = 0;
	Client* pClient;
	DWORD curTime;

	while (totalAccTime <= 400.0)
	{
		if (accTime >= loginTime)
		//if(vecClient.size() != 39)
		{
			printf("생성\n");
			howMany = (rand() % 10) + 1;
			if (howMany + vecClient.size() > MAX_CLIENT_SIZE)
				howMany = MAX_CLIENT_SIZE - vecClient.size();

			howMany = 39;

			while (curClient < howMany)
			{
				pClient = new Client;
				if (!pClient->Init(pServerIP, port)) 
				{
					delete pClient; 
					continue;
				}
				pClient->LobbyTestInit(logoutTime);
				pClient->JustLogin((nickname + std::to_wstring(clientID)).c_str());
				vecClient.push_back(pClient);
				++curClient;
				++clientID;
			}
			accTime = 0.0;
			curClient = 0;
		}

		curTime = timeGetTime();
		double deltaTime = (curTime - prevTime) * 0.001;
		prevTime = curTime;
		accTime += deltaTime; 
		totalAccTime += deltaTime;

		std::vector<Client*>::iterator iter = vecClient.begin();
		std::vector<Client*>::iterator iterEnd = vecClient.end();
		for (; iter != vecClient.end();)
		{
			if (!(*iter)->Update(deltaTime))
			{
				(*iter)->Logout(); // Logout패킷이 도착하기 전에 closesocket해버려서 몇몇 소켓들은 10054 에러
				delete* iter;
				iter = vecClient.erase(iter);
			}
			else
				++iter;
		}
		//printf("경과시간 : %lf\n", totalAccTime);
	}

	std::vector<Client*>::iterator iter = vecClient.begin();
	std::vector<Client*>::iterator iterEnd = vecClient.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Logout();
		delete* iter;
	}
	printf("LobbyTest end! \n");
	system("pause");
}