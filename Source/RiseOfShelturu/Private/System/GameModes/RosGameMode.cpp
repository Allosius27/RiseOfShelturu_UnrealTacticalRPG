// Copyright Epic Games, Inc. All Rights Reserved.


#include "System/GameModes/RosGameMode.h"

#include "System/GameInstances/RosGameManager.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerController.h"
#include "System/Player/RosPlayerState.h"
#include "UI/RosHUD.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Datas/Common/RosUnitData.h"
#include "UObject/ConstructorHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosGameMode)

ARosGameMode::ARosGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	GameStateClass = ARosGameState::StaticClass();
	PlayerControllerClass = ARosPlayerController::StaticClass();
	ReplaySpectatorPlayerControllerClass = ARosReplayPlayerController::StaticClass();
	PlayerStateClass = ARosPlayerState::StaticClass();
	DefaultPawnClass = ARosUnit::StaticClass();
	HUDClass = ARosHUD::StaticClass();
}

const URosUnitData* ARosGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ARosPlayerState* RosPS = InController->GetPlayerState<ARosPlayerState>())
		{
			if (const URosUnitData* PawnData = RosPS->GetUnitData<URosUnitData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void ARosGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

UClass* ARosGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const URosUnitData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ARosGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

bool ARosGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return Super::ShouldSpawnAtStartSpot(Player);
}

void ARosGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

AActor* ARosGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ARosGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ARosGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return Super::PlayerCanRestart_Implementation(Player);
}

void ARosGameMode::InitGameState()
{
	Super::InitGameState();
}

void ARosGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void ARosGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
}

bool ARosGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return Super::UpdatePlayerStartSpot(Player, Portal, OutErrorMessage);
}


void ARosGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	// If we tried to spawn a pawn and it failed, lets try again *note* check if there's actually a pawn class
	// before we try this forever.
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			// If it's a player don't loop forever, maybe something changed and they can no longer restart if so stop trying.
			if (PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);			
			}
			else
			{
				UE_LOG(LogRos, Verbose, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(LogRos, Verbose, TEXT("FailedToRestartPlayer(%s) but there's no pawn class so giving up."), *GetPathNameSafe(NewPlayer));
	}
}

void ARosGameMode::BeginPlay()
{
	Super::BeginPlay();
}



