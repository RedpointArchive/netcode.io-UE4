// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/NetDriver.h"
#if PLATFORM_HTML5
#include <emscripten/val.h>
#endif
#include "NetcodeNetDriver.generated.h"

/**
 * Supports netcode.io protocol for UDP messages in HTML5.
 */
UCLASS()
class NETCODETEST_API UNetcodeNetDriver : public UNetDriver
{
	GENERATED_UCLASS_BODY()

	/** netcode.io server port */
	UPROPERTY(Config)
	int32 NetcodePort;

	//~ Begin UNetDriver Interface.
	virtual bool IsAvailable() const override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual void ProcessRemoteFunction(class AActor* Actor, class UFunction* Function, void* Parameters, struct FOutParmRec* OutParms, struct FFrame* Stack, class UObject* SubObject = NULL) override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void LowLevelSend(FString Address, void* Data, int32 CountBits) override;
	virtual FString LowLevelGetNetworkNumber() override;
	virtual void LowLevelDestroy() override;
	virtual bool IsNetResourceValid(void) override;
	
	// Stub implementaton because we don't use any underlying socket subsystem for netcode.io
	virtual class ISocketSubsystem* GetSocketSubsystem() override;

	//~ End UNetDriver Interface.

	//~ Begin FExec Interface
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar = *GLog) override;
	//~ End FExec Interface

private:
	TArray<uint8> ConnectionToken;

};
