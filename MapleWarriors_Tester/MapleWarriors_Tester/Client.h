#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <array>
#include <string>
#include <random>

enum class eSceneState
{
	None,
	Login,
	Lobby,
	Room,
	InGame,
};

class Client
{
private:
	std::wstring m_nickname;

	char					m_buffer[10240];
	char					m_recvBuffer[10240];
	SOCKET					m_hClientSocket;

	int						m_logoutTime = 0;
	double					m_accTimeUpdateInfo = 0.0;
	double					m_accTimeLobbyChat = 0.0;
	double					m_accTimeLobbyChatMax = 2.0;
	double					m_accTimeCreateRoom = 0.0;
	double					m_accTime = 0.0;

	std::random_device m_rd; 
	std::mt19937 m_gen; 
	std::uniform_real_distribution<> m_dis;

	WSABUF m_buf;
	DWORD m_flags = 0;
	DWORD m_bytesReceived = 0;
	WSAOVERLAPPED m_overlapped = {};

	eSceneState m_eSceneState = eSceneState::Lobby;

	// LobbyChat : 4 + 20 + 2 + 400 + 2 (428)
	// 97��, 181��, 134�� (194bytes, 362bytes, 268bytes) - ������
	std::array<const wchar_t*, 3> m_arrChat{
			L"�� ���޸� �յΰ� ���ࡤ���깰�� ���� �ִ� 60%(���� 30%, ������ü �ִ� 30%) ���� ������ �����ȴ�. �̸� ���� ���� �������� �������� 10%����Ʈ ���������ߴ�.",
			L"15��(�����ð�) ���ֱ�� �ֽ�Ʈ�κ�ƽ�� ������ �� ȸ�簡 ������ ���� 8�� ���ַ� �߻�� �� ������ �䷹�׸��� ���ǿ� �������� ��Ÿ ������� ������� �ӹ��� ������ �����̴�. �� ȸ��� �� �װ����ֱ�(NASA)�� ������ ���� �а�� �� ���ηκ��� ���� �����ϰ� å�Ӱ� �ִ� ��ġ�� ���� �ǰ��� �޾Ҵٸ鼭 �̰��� ������fdsafdsafdsafdsaweq��.", // �ִ� 200�� + ��
			L"In 1965, I. J. Good wrote that it is more probable than not that an ultraintelligent machine would be built in the twentieth century."
	};

public:
	Client(HANDLE _hCPObject);
	~Client();

	bool Update(double _deltaTime);

	bool Init(const char* _pServerIP, int _serverPort);
	void LobbyTestInit(int _logoutTime);

	void LobbyUpdateInfo();
	void JustLogin(const wchar_t* _pNickname);
	void LobbyChat(const wchar_t* _pChat);
	void RoomChat(const wchar_t* _pChat);
	void CreateRoom();
	void Shutdown();
	void CloseSocket();

	void RegisterRecv();

	double GetRandomNumber() { return m_dis(m_gen); }
	int GetSocket() const { return (int)m_hClientSocket; }
};


