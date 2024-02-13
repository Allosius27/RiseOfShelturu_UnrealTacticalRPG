// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "RosLocalPlayer.generated.h"

class APlayerController;
class UInputMappingContext;
class UObject;
class UWorld;
struct FFrame;

/**
 * URosLocalPlayer
 */
UCLASS()
class RISEOFSHELTURU_API URosLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:

	URosLocalPlayer();

#pragma region PROPERTIES

public:

private:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface
	
protected:

private:
	void OnPlayerControllerChanged(APlayerController* NewController);

#pragma endregion
};
