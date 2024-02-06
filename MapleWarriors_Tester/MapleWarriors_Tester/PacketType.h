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
	CreateRoom_Success,
	CreateRoom_Fail,
	EnterRoom_Success,
	EnterRoom_Full,
	EnterRoom_InGame,
	EnterRoom_NoRoom,
	ExitRoom,
	StartGame_Success,
	StartGame_Fail,
	NotifyRoomUserEnter,
	NotifyRoomUserExit,
	RoomChat,
	RoomUsersInfo,
	RoomReady,
	RoomReady_Fail,
	RoomStandby,
	RoomStandby_Fail,
	ResInitInfo,
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
	EnterRoom,
	ExitRoom,
	StartGame,
	RoomReady,
	RoomStandby,
	RoomChat,
	ReqRoomUsersInfo,
	ReqInitInfo,
};