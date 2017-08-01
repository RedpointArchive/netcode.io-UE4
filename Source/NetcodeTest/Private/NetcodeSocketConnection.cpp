// Fill out your copyright notice in the Description page of Project Settings.

#include "NetcodeSocketConnection.h"
#ifndef PLATFORM_HTML5
#include "netcode.h"
#endif

UNetcodeSocketConnection::UNetcodeSocketConnection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_LIBSODIUM_BINDING
	this->NetcodeClient = nullptr;
#elif PLATFORM_HTML5
	this->IsClientReady = false;
#endif
}

void UNetcodeSocketConnection::InitBase(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
}

void UNetcodeSocketConnection::InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
}

void UNetcodeSocketConnection::InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
}

void UNetcodeSocketConnection::LowLevelSend(void* Data, int32 CountBytes, int32 CountBits)
{
}

FString UNetcodeSocketConnection::LowLevelGetRemoteAddress(bool bAppendPort)
{
	return TEXT("");
}

FString UNetcodeSocketConnection::LowLevelDescribe()
{
	return TEXT("");
}

int32 UNetcodeSocketConnection::GetAddrAsInt()
{
	return 0;
}

int32 UNetcodeSocketConnection::GetAddrPort()
{
	return 0;
}

FString UNetcodeSocketConnection::RemoteAddressToString()
{
	return TEXT("");
}

void UNetcodeSocketConnection::Tick()
{
}

void UNetcodeSocketConnection::FinishDestroy()
{
}

void UNetcodeSocketConnection::ReceivedRawPacket(void* Data, int32 Count)
{
}

#if WITH_LIBSODIUM_BINDING

void UNetcodeSocketConnection::SetNetcodeClient(struct netcode_client_t * Client)
{
	this->NetcodeClient = Client;
}

struct netcode_client_t * UNetcodeSocketConnection::GetNetcodeClient()
{
	return this->NetcodeClient;
}

#endif

bool UNetcodeSocketConnection::IsReady()
{
#if PLATFORM_HTML5
	return this->IsClientReady;
#else
	// Connection is synchronously setup when socket is constructed by net driver.
	return true;
#endif
}