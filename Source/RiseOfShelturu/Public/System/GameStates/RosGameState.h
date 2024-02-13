// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameState.h"
#include "System/RosSystemTypes.h"
#include "RosGameState.generated.h"

class ARosUnit;
class APlayerState;
class UAbilitySystemComponent;
class URosAbilitySystemComponent;
class UObject;
struct FFrame;

/**
 * ARosGameState
 *
 *	The base game state class used by this project.
 */
UCLASS(Config = Game)
class RISEOFSHELTURU_API ARosGameState : public AGameState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ARosGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle", meta = (DisplayPriority = 0))
	TArray<ARosUnit*> TempBattleUnits;

protected:

	UPROPERTY(Replicated)
	float ServerFPS;
	

	UPROPERTY(BlueprintGetter=GetCurrentGamePhase, VisibleAnywhere, Category = "GameState|GamePhase", meta = (DisplayPriority = 0))
	ERosGamePhase CurrentGamePhase = ERosGamePhase::E_NonePhase;
	
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle", meta = (DisplayPriority = 0))
	TArray<ARosUnit*> NextBattleUnitsToAdd;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle", meta = (DisplayPriority = 0))
	TArray<ARosUnit*> DeletedBattleUnits;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle", meta = (DisplayPriority = 0))
	TArray<ARosUnit*> CurrentUnitsTurn;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle")
	int TurnsCount;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "GameState|GamePhase|Battle")
	int CurrentTurnIndex;

private:

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	TObjectPtr<URosAbilitySystemComponent> AbilitySystemComponent;

#pragma endregion

#pragma region EVENTS

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSetGamePhaseSignature OnSetGamePhase;

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	float GetServerFPS() const { return ServerFPS; }
	
	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	UFUNCTION(BlueprintCallable, Category = "GameState")
	URosAbilitySystemComponent* GetRosAbilitySystemComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "GameState|GamePhase")
	ERosGamePhase GetCurrentGamePhase() const { return CurrentGamePhase; }

#pragma endregion
	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase")
	void SetGamePhase(ERosGamePhase newGamePhase);

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase")
	void BroadcastOnSetGamePhase();

	
	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Exploration")
	void SetExplorationPhase();

	
	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void SetBattlePhase();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void InsertTempUnitsToBattle();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void NextTurn();
	
protected:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void ResetBattleParameters();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void AddPlayerTeamToBattleTurn();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void AddTempUnitsToNextBattleIntegration();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void ExecuteAddNextUnitsToBattle();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void SetStartUnitTurn();

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void SetUnitTurn(ARosUnit* unitTurn);

	UFUNCTION(BlueprintCallable, Category = "GameState|GamePhase|Battle")
	void SetNewRoundTurn();

#pragma endregion
};
