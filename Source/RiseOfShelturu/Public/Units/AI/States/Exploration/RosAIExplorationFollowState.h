// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/AI/States/RosAIBaseState.h"
#include "RosAIExplorationFollowState.generated.h"

class ARosHeroUnit;
class UUtilityCustomTimer;
class URosAIStateMachineBrain;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosAIExplorationFollowState : public URosAIBaseState
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosAIExplorationFollowState();

#pragma region PROPERTIES

public:

protected:

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetMoveCooldownTimer, Category= "ExplorationFollowState|Timer")
	UUtilityCustomTimer* MoveCooldownTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "ExplorationFollowState|Target")
	ARosHeroUnit* TargetUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "ExplorationFollowState|Target")
	int TargetSpotPointIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "ExplorationFollowState|Target")
	int HasTargetSpotPoint;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable)
	UUtilityCustomTimer* GetMoveCooldownTimer() const { return MoveCooldownTimer; }

#pragma endregion

	virtual void OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner) override;

	virtual void OnEnter() override;

	virtual void OnExit() override;

	virtual void OnTick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void OnStartMove();

	UFUNCTION(BlueprintCallable)
	void OnEndMoveWaiting();
	
protected:

#pragma endregion
};
