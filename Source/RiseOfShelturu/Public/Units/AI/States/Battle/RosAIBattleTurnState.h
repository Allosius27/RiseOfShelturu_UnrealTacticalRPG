// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/AI/States/RosAIBaseState.h"
#include "RosAIBattleTurnState.generated.h"

class UUtilityCustomTimer;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosAIBattleTurnState : public URosAIBaseState
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosAIBattleTurnState();

#pragma region PROPERTIES

public:

protected:

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetWaitingTurnTimer, Category= "BattleAITurnState|Timer")
	UUtilityCustomTimer* WaitingTurnTimer;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable)
	UUtilityCustomTimer* GetWaitingTurnTimer() const { return WaitingTurnTimer; }

#pragma endregion

	virtual void OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner) override;

	virtual void OnEnter() override;

	virtual void OnExit() override;

	virtual void OnTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnEndWaitingTurn();
	
protected:

#pragma endregion
};
