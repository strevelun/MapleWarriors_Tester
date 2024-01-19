#pragma once


enum class eServer 
{
	None = 0,
	Test,
	LoginFailure_AlreadyLoggedIn,
	LoginFailure_Full,
	LoginSuccess,
	LobbyChat,
	LobbyUpdateInfo_UserList,
	LobbyUpdateInfo_RoomList,
	CreateRoom,
};

enum class eClient  
{
	None = 0,
	Test,
	Exit,
	LoginReq,
	LobbyChat,
	LobbyUpdateInfo,
	UserListGetPageInfo,
	RoomListGetPageInfo,
	CreateRoom,
};