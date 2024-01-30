#include "Client.h"
#include "PacketType.h"

#include <iostream>

typedef unsigned short ushort;

Client::Client(HANDLE _hCPObject)  :
	m_gen(m_rd()), m_dis(0.1, 1.5)
{
	m_hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	HANDLE h = CreateIoCompletionPort((HANDLE)m_hClientSocket, _hCPObject, (ULONG_PTR)this, 0);

	m_buf.len = sizeof(m_recvBuffer);
	m_buf.buf = m_recvBuffer;
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
	m_accTimeCreateRoom += _deltaTime;

	if (m_accTimeUpdateInfo >= 5.0)
	{
		m_accTimeUpdateInfo = 0.0;
		LobbyUpdateInfo();
	}
	
	if (m_accTimeLobbyChat >= m_accTimeLobbyChatMax)
	{
		m_accTimeLobbyChat = 0.0f;
		m_accTimeLobbyChatMax = GetRandomNumber();
		LobbyChat(m_arrChat[1]);
	}

	if (m_accTimeCreateRoom >= rand() % 10 + 1)
	{
		m_accTimeCreateRoom = -999999.0;
		CreateRoom();
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
	//printf("LobbyUpdateInfo");
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LobbyUpdateInfo;						count += sizeof(ushort);
	*(char*)(m_buffer + count) = 0;							count += sizeof(char);
	*(char*)(m_buffer + count) = 0;							count += sizeof(char);
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::JustLogin(const wchar_t* _pNickname)
{
	m_nickname = _pNickname;
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LoginReq;						count += sizeof(ushort);
	memcpy(m_buffer + count, _pNickname, wcslen(_pNickname) * 2);						count += (ushort)wcslen(_pNickname) * 2;
	*(wchar_t*)(m_buffer + count) = L'\0';											count += 2;
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::LobbyChat(const wchar_t* _pChat)
{
	//printf("LobbyChat");
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::LobbyChat;						count += sizeof(ushort);
	memcpy(m_buffer + count, _pChat, wcslen(_pChat) * 2);						count += (ushort)wcslen(_pChat) * 2;
	*(wchar_t*)(m_buffer + count) = L'\0';											count += 2;
	*(ushort*)m_buffer = count;
	int result = send(m_hClientSocket, m_buffer, count, 0);
	//printf("LobbyChat : %d\n", result);
}

void Client::CreateRoom()
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::CreateRoom;						count += sizeof(ushort);
	memcpy(m_buffer + count, m_nickname.c_str(), wcslen(m_nickname.c_str()) * 2);						count += (ushort)wcslen(m_nickname.c_str()) * 2;
	*(wchar_t*)(m_buffer + count) = L'\0';											count += 2;
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::Logout()
{
	ushort count = sizeof(ushort);
	*(ushort*)(m_buffer + count) = (ushort)eClient::Exit;						count += sizeof(ushort);
	*(ushort*)m_buffer = count;
	send(m_hClientSocket, m_buffer, *(u_short*)m_buffer, 0);
}

void Client::CloseSocket()
{
	closesocket(m_hClientSocket);
	m_hClientSocket = INVALID_SOCKET;
}

void Client::RegisterRecv()
{
	DWORD flags = 0;
	DWORD bytesReceived = 0;

	if (m_hClientSocket == INVALID_SOCKET) return;

	int result = WSARecv(m_hClientSocket, &m_buf, 1, &m_bytesReceived, &m_flags, &m_overlapped, nullptr);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			printf("WSARecv failed with error: %d\n", error);
	}
	//printf("RegisterRecv\n");
}