// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/RosCheatManager.h"

#include "Engine/Console.h"
#include "System/Player/RosPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosCheatManager)

DEFINE_LOG_CATEGORY(LogRosCheat);

namespace RosCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("RosCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("RosCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
}

URosCheatManager::URosCheatManager()
{
	
}

void URosCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		
	}
#endif

	if (RosCheat::bStartInGodMode)
	{
		God();	
	}
}

void URosCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogRosCheat, Display, TEXT("%s"), *TextToOutput);
#endif // USING_CHEAT_MANAGER
}

void URosCheatManager::Cheat(const FString& Msg)
{
	
}

void URosCheatManager::CheatAll(const FString& Msg)
{
	
}

void URosCheatManager::DamageSelf(float DamageAmount)
{
	
}

void URosCheatManager::DamageTarget(float DamageAmount)
{
	Super::DamageTarget(DamageAmount);
}

void URosCheatManager::HealSelf(float HealAmount)
{
	
}

void URosCheatManager::HealTarget(float HealAmount)
{
	
}

void URosCheatManager::DamageSelfDestruct()
{
	
}

void URosCheatManager::God()
{
	Super::God();
}

void URosCheatManager::UnlimitedHealth(int32 Enabled)
{
	
}
