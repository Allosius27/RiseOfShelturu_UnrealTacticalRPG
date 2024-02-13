// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameInstances/RosGameManager.h"

#include "System/Player/RosPlayerController.h"

DEFINE_LOG_CATEGORY(LogRos);

URosGameManager::URosGameManager(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

ARosPlayerController* URosGameManager::GetPrimaryPlayerController() const
{
	return Cast<ARosPlayerController>(Super::GetPrimaryPlayerController(false));
}

void URosGameManager::SetDrawDebugAllow(bool value)
{
	DrawDebugAllow = value;
}
