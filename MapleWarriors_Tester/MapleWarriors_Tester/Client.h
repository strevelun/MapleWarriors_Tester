#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <array>

class Client
{
private:
	char					m_buffer[1024];
	SOCKET					m_hClientSocket;

	int						m_logoutTime = 0;
	double					m_accTimeUpdateInfo = 0.0;
	double					m_accTimeLobbyChat = 0.0;
	double					m_accTime = 0.0;

	// 97, 181, 134 (194, 362, 268) - ������
	std::array<const wchar_t*, 3> m_arrChat{
			L"�� ���޸� �յΰ� ���ࡤ���깰�� ���� �ִ� 60%(���� 30%, ������ü �ִ� 30%) ���� ������ �����ȴ�. �̸� ���� ���� �������� �������� 10%����Ʈ ���������ߴ�.",
			L"15��(�����ð�) ���ֱ�� �ֽ�Ʈ�κ�ƽ�� ������ �� ȸ�簡 ������ ���� 8�� ���ַ� �߻�� �� ������ �䷹�׸��� ���ǿ� �������� ��Ÿ ������� ������� �ӹ��� ������ �����̴�. �� ȸ��� �� �װ����ֱ�(NASA)�� ������ ���� �а�� �� ���ηκ��� ���� �����ϰ� å�Ӱ� �ִ� ��ġ�� ���� �ǰ��� �޾Ҵٸ鼭 �̰��� ������.",
			L"In 1965, I. J. Good wrote that it is more probable than not that an ultraintelligent machine would be built in the twentieth century."
	};

public:
	Client();
	~Client();

	bool Update(double _deltaTime);

	bool Init(const char* _pServerIP, int _serverPort);
	void LobbyTestInit(int _logoutTime);

	void LobbyUpdateInfo();
	void JustLogin(const wchar_t* _pNickname);
	void LobbyChat(const wchar_t* _pChat);
	void Logout();
};


