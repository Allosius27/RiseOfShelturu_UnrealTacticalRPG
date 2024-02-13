// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "System/RosSystemTypes.h"
#include "RosGameMode.generated.h"

class URosGameData;
class URosUnitData;
class AActor;
class AController;
class AGameModeBase;
class APawn;
class APlayerController;
class UClass;
class UObject;
struct FFrame;
struct FPrimaryAssetId;
enum class ECommonSessionOnlineMode : uint8;

/**
 * ARosGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class ARosGameMode : public AGameMode
{
	GENERATED_BODY()
	
	ARosGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, Category = "GameMode|Datas")
	TObjectPtr<const URosGameData> GameData;

#pragma endregion

#pragma region EVENTS

public:

	// Delegate called on player initialization, described above 
	FOnRosGameModePlayerInitialized OnGameModePlayerInitialized;

#pragma endregion

#pragma region FUNCTIONS

public:
	
	UFUNCTION(BlueprintCallable, Category = "GameMode|Pawn")
	const URosUnitData* GetPawnDataForController(const AController* InController) const;

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void InitGameState() override;
	virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	virtual void GenericPlayerInitialization(AController* NewPlayer) override;
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	virtual void BeginPlay() override;

	// Restart (respawn) the specified player or bot next frame
	// - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);
	
protected:

#pragma endregion
};



