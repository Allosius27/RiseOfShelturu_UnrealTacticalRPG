// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/AI/States/RosAIBaseState.h"
#include "RosExplorationBattleDetectState.generated.h"

class ARosGameState;
class ARosHeroUnit;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosExplorationBattleDetectState : public URosAIBaseState
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosExplorationBattleDetectState();

#pragma region PROPERTIES

public:

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "ExplorationBattleDetectState")
	bool CanDetect = true;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion

	virtual void OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner) override;

	virtual void OnEnter() override;

	virtual void OnExit() override;

	virtual void OnTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnBattleDetectHero(ARosHeroUnit* hero, bool canLaunchBattle);
	
protected:

#pragma endregion
};
