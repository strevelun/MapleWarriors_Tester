#include "Client.h"
#include "PacketType.h"

#include <iostream>

typedef unsigned short ushort;

Client::Client() 
{
	m_hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

Client::~Client()
{
	if (m_hClientSocket != INVALID_SOCKET) closesocket(m_hClientSocket);
}

bool Client::Update(double _deltaTime)
{
	m_accTime += _deltaTime;
	m_accTimeUpdateInfo += _deltaTime;
	m_accTimeLobbyChat += _deltaTime;

	if (m_accTimeUpdateInfo >= 5.0)
	{
		m_accTimeUpdateInfo = 0.0;
		LobbyUpdateInfo();
	}
	
	if (m_accTimeLobbyChat >= 1.0)
	{
		m_accTimeLobbyChat = 0.0f;
		LobbyChat(m_arrChat[(rand() % 3)]);
	}
	
	if (m_accTime >= m_logoutTime) return false;

	return true;
}

bool Client::Init(const char* _pServerIP, int _serverPort)
{
	SOCKADDR_IN				servAddr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, _pServerIP, &servAddr.sin_addr);
	servAddr.sin_port = htons(_serverPort);

	if (connect(m_hClientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << "connect 에러코드 : " << error << '\n';
		return false;
	}

	return true;
}

void Client::LobbyTestInit(int _logoutTime)
{
	m_logoutTime = _logoutTime;
}

void Client::LobbyUpdateInfo()
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LobbyUpdateInfo;						count += sizeof(ushort);
	*(char*)(m_buffer + count) = 0;							count += sizeof(char);
	*(char*)(m_buffer + count) = 0;							count += sizeof(char);
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::JustLogin(const wchar_t* _pNickname)
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LoginReq;						count += sizeof(ushort);
	memcpy(m_buffer + count, _pNickname, wcslen(_pNickname) * 2);						count += (ushort)wcslen(_pNickname) * 2;
	*(wchar_t*)(m_buffer + count) = L'\0';											count += 2;
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::LobbyChat(const wchar_t* _pChat)
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LobbyChat;						count += sizeof(ushort);
	memcpy(m_buffer + count, _pChat, wcslen(_pChat) * 2);						count += (ushort)wcslen(_pChat) * 2;
	*(wchar_t*)(m_buffer + count) = L'\0';											count += 2;
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, count, 0);
}

void Client::Logout()
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::Exit;						count += sizeof(ushort);
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}
