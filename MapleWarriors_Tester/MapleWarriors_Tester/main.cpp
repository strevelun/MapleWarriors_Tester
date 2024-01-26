#include "Client.h"

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <process.h>

#pragma comment(lib, "winmm.lib") 
#pragma comment( lib, "ws2_32.lib")

#define MAX_CLIENT_SIZE			21
#define USER_NUM				1		// �׽��� Ŭ���̾�Ʈ�� �����ϰ� �ٷ� SendChat�ϱ� ������ ������ �� �� ������ �ݵ�� ���� ����� �� �Է�

double accTime = 0.0;
std::wstring nickname = L"Test";
std::wstring roomName = L"Room";
HANDLE hCPObject;

void LobbyTest();

enum class eChoice
{
	None,
	LobbyTest,
	ChoiceMax
};
const char* pServerIP = "192.168.219.105";
//const char* pServerIP = "220.121.252.109"; // gpm
//const char* pServerIP = "220.121.252.11"; // gpm
// const char * pServerIP = "220.127.242.178";
const int port = 30001;

unsigned int __stdcall Worker(void* _pArgs)
{
	HANDLE hIOCP = (HANDLE)_pArgs;
	DWORD			bytesTransferred = 0;
	WSAOVERLAPPED* pOverlapped = nullptr;
	Client* pClient = nullptr;

	while (1)
	{
		//if(pConn)
		//	("[%d] GetQUeuedCS ȣ�� ��\n", (int)pConn->GetSocket());
		bool result = GetQueuedCompletionStatus(hIOCP, &bytesTransferred, (PULONG_PTR)&pClient, (LPOVERLAPPED*)&pOverlapped, INFINITE);
		if (!result)
		{
			printf("false returned : %d\n", WSAGetLastError());
			continue;
		}

		printf("%d\n", bytesTransferred);

		if (bytesTransferred == 0)
		{
			continue;
		}

		pClient->RegisterRecv();
	}
	return 0;
}

int main()
{
	SYSTEM_INFO		si;
	GetSystemInfo(&si);
	hCPObject = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	HANDLE t;
	for (int i = 0; i < si.dwNumberOfProcessors * 2; ++i)
	{
		t = (HANDLE)_beginthreadex(nullptr, 0, &Worker, (void*)hCPObject, 0, nullptr);
		if (t == 0)
		{
			printf("thread ���� ����\n");
			return false;
		}
	}

	srand((unsigned int)time(nullptr));

	WSADATA  wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;

	int clientCount = 0;
	int input;
	while (1)
	{
		system("cls");
		std::cout << "����(�κ��׽�Ʈ : 1) : ";
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
	// ������ �ð��� ������ ������ Ŭ���̾�Ʈ ���� or �α׾ƿ�
	double totalAccTime = 0.0;
	std::vector<Client*>			vecClient;
	std::vector<Client*>			vecClientTrash;
	int clientID = 0;

	DWORD prevTime = timeGetTime();

	int loginTime = (rand() % 4) + 1;
	//int logoutTime = (rand() % 30) + 1;
	int logoutTime = 9999;

	int howMany, curClient = 0;
	Client* pClient;
	DWORD curTime;

	while (totalAccTime <= 3600.0)
	{
		if (accTime >= loginTime)
		//if(vecClient.size() != 9)
		{
			howMany = (rand() % MAX_CLIENT_SIZE - USER_NUM) + 1;
			if (vecClient.size() >= MAX_CLIENT_SIZE - USER_NUM) howMany = 0;
			else if (howMany + vecClient.size() > MAX_CLIENT_SIZE - USER_NUM) // 296+3
				howMany = MAX_CLIENT_SIZE - USER_NUM - vecClient.size(); // 300-2-296

			//printf("���� : %d��\n", howMany);
			//howMany = 9;

			while (curClient < howMany)
			{
				pClient = new Client(hCPObject);
				if (!pClient->Init(pServerIP, port)) 
				{
					delete pClient; 
					continue;
				}
				pClient->RegisterRecv();
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
				(*iter)->Logout(); // Logout��Ŷ�� �����ϱ� ���� closesocket�ع����� ��� ���ϵ��� 10054 ����
				vecClientTrash.push_back(*iter);
				(*iter)->CloseSocket();
				iter = vecClient.erase(iter);
			}
			else
				++iter;
		}

		SleepEx(0, TRUE);
		//printf("����ð� : %lf\n", totalAccTime);
	}
	//printf("�ð� ��!\n");
	//Sleep(1000000);
	std::vector<Client*>::iterator iter = vecClient.begin();
	std::vector<Client*>::iterator iterEnd = vecClient.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Logout();				
		vecClientTrash.push_back(*iter);
	}

	for (Client* c : vecClientTrash)
		delete c;
	printf("LobbyTest end! \n");
	system("pause");
}