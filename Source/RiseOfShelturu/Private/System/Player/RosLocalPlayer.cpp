// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/RosLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosLocalPlayer)

class UObject;

URosLocalPlayer::URosLocalPlayer()
{
}

void URosLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
}

void URosLocalPlayer::SwitchController(APlayerController* PC)
{
	Super::SwitchController(PC);
	
	OnPlayerControllerChanged(PlayerController);
}

bool URosLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void URosLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);
	
	Super::InitOnlineSession();
}

void URosLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	
}

