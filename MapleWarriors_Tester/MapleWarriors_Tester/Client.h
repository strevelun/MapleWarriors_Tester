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
	// 97개, 181개, 134개 (194bytes, 362bytes, 268bytes) - 널포함
	std::array<const wchar_t*, 3> m_arrChat{
			L"설 연휴를 앞두고 농·축·수산물에 대해 최대 60%(정부 30%, 업계자체 최대 30%) 할인 혜택이 제공된다. 이를 위해 정부 할인폭을 기존보다 10%포인트 상향조정했다.",
			L"15일(현지시간) 우주기업 애스트로보틱에 따르면 이 회사가 개발해 지난 8일 우주로 발사된 달 착륙선 페레그린은 대기권에 재진입해 불타 사라지는 방식으로 임무를 종료할 예정이다. 이 회사는 미 항공우주국(NASA)과 협력해 우주 학계와 미 정부로부터 가장 안전하고 책임감 있는 조치에 대한 의견을 받았다면서 이같이 밝혔다fdsafdsafdsafdsaweqㅇ.", // 최대 200자 + 널
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


