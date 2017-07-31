// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/NetConnection.h"
#include "netcode.h"
#include "NetcodeSocketConnection.generated.h"

/**
 * Represents a netcode.io connection.
 */
UCLASS(transient, config = Engine)
class NETCODETEST_API UNetcodeSocketConnection : public UNetConnection
{
	GENERATED_UCLASS_BODY()

	//~ Begin NetConnection Interface
	virtual void InitBase(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitRemoteConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, const class FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitLocalConnection(UNetDriver* InDriver, class FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void LowLevelSend(void* Data, int32 CountBytes, int32 CountBits) override;
	FString LowLevelGetRemoteAddress(bool bAppendPort = false) override;
	FString LowLevelDescribe() override;
	virtual int32 GetAddrAsInt(void) override;
	virtual int32 GetAddrPort(void) override;
	virtual FString RemoteAddressToString() override;
	virtual void Tick();
	virtual void FinishDestroy();
	virtual void ReceivedRawPacket(void* Data, int32 Count);
	//~ End NetConnection Interface

#if WITH_LIBSODIUM_BINDING
	void SetNetcodeClient(struct netcode_client_t * Client);
	struct netcode_client_t * GetNetcodeClient();

private:

	struct netcode_client_t * NetcodeClient;
#endif
};