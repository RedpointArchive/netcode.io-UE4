// Fill out your copyright notice in the Description page of Project Settings.

#include "NetcodeNetDriver.h"
#include "NetcodeSocketConnection.h"
#include "netcode.h"

#include "Base64.h"
#include "SocketSubsystem.h"

UNetcodeNetDriver::UNetcodeNetDriver(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UNetcodeNetDriver::IsAvailable() const
{
	// We need to be either HTML5 with the Javascript client API or have libsodium
	// available for things to work
#if PLATFORM_HTML5 || WITH_LIBSODIUM_BINDING
	return true;
#else
	return false;
#endif
}

ISocketSubsystem* UNetcodeNetDriver::GetSocketSubsystem()
{
	return ISocketSubsystem::Get();
}

bool UNetcodeNetDriver::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	if (!Super::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	return true;
}

bool UNetcodeNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	if (!InitBase(true, InNotify, ConnectURL, false, Error))
	{
		return false;
	}

#if PLATFORM_HTML5 || WITH_LIBSODIUM_BINDING
	// Create a new instance for the connection to the server.
	ServerConnection = NewObject<UNetcodeSocketConnection>(NetConnectionClass);

	// Read the connection URL.
	TArray<uint8> ConnectionTokenArray;
	uint8_t* ConnectionToken;
	const TCHAR* const TokenRaw = ConnectURL.GetOption(TEXT("Token="), nullptr);
	FString Token = TEXT("");
	if (TokenRaw != nullptr)
	{
		Token = FString(TokenRaw);
	}
	if (Token.IsEmpty())
	{
#if PLATFORM_HTML5
		// HTML5 can not connect to unauthenticated servers - token must be provided.
		ServerConnection = NULL;
		return false;
#else
		// We have no token, which means we're communicating locally; just use the empty
		// array as the private key.
		ConnectionToken = ConnectionTokenArray.GetData();
#endif
	}
	else
	{
		// Decode the token from a Base64 value.
		FBase64::Decode(Token, ConnectionTokenArray);
		ConnectionToken = ConnectionTokenArray.GetData();
	}

#if PLATFORM_HTML5
	EM_ASM(
		// TODO: Use Javascript APIs to set up netcode.io client.
		/*window.netcode.createClient('ipv4', function(err, client) {
			
		});*/
	)
#else
	// Create the netcode.io client.
	struct netcode_client_t* Client = netcode_client_create("0.0.0.0", 0);

	// Assign the client to the connection.
	UNetcodeSocketConnection* NetcodeConnection = (UNetcodeSocketConnection*)ServerConnection;
	NetcodeConnection->SetNetcodeClient(Client);

	// Tell netcode.io to connect.
	netcode_client_connect(Client, ConnectionToken);

	return true;
#endif
	
#endif

	return false;

	/*
	// Create new connection.
	ServerConnection = NewObject<UWebSocketConnection>(NetConnectionClass);

	TSharedRef<FInternetAddr> InternetAddr = GetSocketSubsystem()->CreateInternetAddr();
	bool Ok;
	InternetAddr->SetIp(*ConnectURL.Host, Ok);
	InternetAddr->SetPort(WebSocketPort);

	FWebSocket* WebSocket = new FWebSocket(*InternetAddr);
	UWebSocketConnection* Connection = (UWebSocketConnection*)ServerConnection;
	Connection->SetWebSocket(WebSocket);

	FWebsocketPacketRecievedCallBack CallBack;
	CallBack.BindUObject(Connection, &UWebSocketConnection::ReceivedRawPacket);
	WebSocket->SetRecieveCallBack(CallBack);

	FWebsocketInfoCallBack  ConnectedCallBack;
	ConnectedCallBack.BindUObject(this, &UWebSocketNetDriver::OnWebSocketServerConnected);
	WebSocket->SetConnectedCallBack(ConnectedCallBack);

	ServerConnection->InitLocalConnection(this, NULL, ConnectURL, USOCK_Pending);

	// Create channel zero.
	GetServerConnection()->CreateChannel(CHTYPE_Control, 1, 0);

	return true;
	*/
}

bool UNetcodeNetDriver::InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error)
{
	if (!InitBase(false, InNotify, LocalURL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	return false;

	/*
	InitConnectionlessHandler();

	WebSocketServer = new class FWebSocketServer();

	FWebsocketClientConnectedCallBack CallBack;
	CallBack.BindUObject(this, &UWebSocketNetDriver::OnWebSocketClientConnected);

	if (!WebSocketServer->Init(WebSocketPort, CallBack))
		return false;

	WebSocketServer->Tick();
	LocalURL.Port = WebSocketPort;
	UE_LOG(LogHTML5Networking, Log, TEXT("%s WebSocketNetDriver listening on port %i"), *GetDescription(), LocalURL.Port);

	// server has no server connection.
	ServerConnection = NULL;
	return true;
	*/
}

void UNetcodeNetDriver::TickDispatch(float DeltaTime)
{
	Super::TickDispatch(DeltaTime);

	/*
	if (WebSocketServer)
		WebSocketServer->Tick();
		*/
}

void UNetcodeNetDriver::LowLevelSend(FString Address, void* Data, int32 CountBits)
{
	/*
	bool bValidAddress = !Address.IsEmpty();
	TSharedRef<FInternetAddr> RemoteAddr = GetSocketSubsystem()->CreateInternetAddr();

	if (bValidAddress)
	{
		RemoteAddr->SetIp(*Address, bValidAddress);
	}

	if (bValidAddress)
	{
		const uint8* DataToSend = reinterpret_cast<uint8*>(Data);

		if (ConnectionlessHandler.IsValid())
		{
			const ProcessedPacket ProcessedData =
				ConnectionlessHandler->OutgoingConnectionless(Address, (uint8*)DataToSend, CountBits);

			if (!ProcessedData.bError)
			{
				DataToSend = ProcessedData.Data;
				CountBits = ProcessedData.CountBits;
			}
			else
			{
				CountBits = 0;
			}
		}


		// connectionless websockets do not exist (yet)
		// scan though existing connections
		if (CountBits > 0)
		{
			for (int32 i = 0; i<ClientConnections.Num(); ++i)
			{
				UWebSocketConnection* Connection = (UWebSocketConnection*)ClientConnections[i];
				if (Connection && (Connection->LowLevelGetRemoteAddress(true) == Address))
				{
					Connection->GetWebSocket()->Send((uint8*)DataToSend, FMath::DivideAndRoundUp(CountBits, 8));
					break;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogNet, Warning, TEXT("UWebSocketNetDriver::LowLevelSend: Invalid send address '%s'"), *Address);
	}*/
}

void UNetcodeNetDriver::ProcessRemoteFunction(class AActor* Actor, UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack, class UObject* SubObject)
{
	/*
	bool bIsServer = IsServer();

	UNetConnection* Connection = NULL;
	if (bIsServer)
	{
		if ((Function->FunctionFlags & FUNC_NetMulticast))
		{
			// Multicast functions go to every client
			TArray<UNetConnection*> UniqueRealConnections;
			for (int32 i = 0; i<ClientConnections.Num(); ++i)
			{
				Connection = ClientConnections[i];
				if (Connection)
				{
					// Do relevancy check if unreliable.
					// Reliables will always go out. This is odd behavior. On one hand we wish to garuntee "reliables always get there". On the other
					// hand, replicating a reliable to something on the other side of the map that is non relevant seems weird.
					//
					// Multicast reliables should probably never be used in gameplay code for actors that have relevancy checks. If they are, the
					// rpc will go through and the channel will be closed soon after due to relevancy failing.

					bool IsRelevant = true;
					if ((Function->FunctionFlags & FUNC_NetReliable) == 0)
					{
						if (Connection->ViewTarget)
						{
							FNetViewer Viewer(Connection, 0.f);
							IsRelevant = Actor->IsNetRelevantFor(Viewer.InViewer, Viewer.ViewTarget, Viewer.ViewLocation);
						}
						else
						{
							// No viewer for this connection(?), just let it go through.
							UE_LOG(LogHTML5Networking, Log, TEXT("Multicast function %s called on actor %s when a connection has no Viewer"), *Function->GetName(), *Actor->GetName());
						}
					}

					if (IsRelevant)
					{
						if (Connection->GetUChildConnection() != NULL)
						{
							Connection = ((UChildConnection*)Connection)->Parent;
						}

						InternalProcessRemoteFunction(Actor, SubObject, Connection, Function, Parameters, OutParms, Stack, bIsServer);
					}
				}
			}

			// Return here so we don't call InternalProcessRemoteFunction again at the bottom of this function
			return;
		}
	}

	// Send function data to remote.
	Connection = Actor->GetNetConnection();
	if (Connection)
	{
		InternalProcessRemoteFunction(Actor, SubObject, Connection, Function, Parameters, OutParms, Stack, bIsServer);
	}
	*/
}

FString UNetcodeNetDriver::LowLevelGetNetworkNumber()
{
	return TEXT("");
	//return WebSocketServer->Info();
}

void UNetcodeNetDriver::LowLevelDestroy()
{
	Super::LowLevelDestroy();
	//delete WebSocketServer;
	//WebSocketServer = nullptr;
}

bool UNetcodeNetDriver::IsNetResourceValid(void)
{
	/*if ((WebSocketServer && !ServerConnection)//  Server
	|| (!WebSocketServer && ServerConnection) // client
	)
	{
	return true;
	}*/

	return false;
}

bool UNetcodeNetDriver::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return UNetDriver::Exec(InWorld, Cmd, Ar);
}