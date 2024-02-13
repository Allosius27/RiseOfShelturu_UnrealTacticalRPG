// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/AI/States/RosAIBaseState.h"
#include "RosAIExplorationWaitingState.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosAIExplorationWaitingState : public URosAIBaseState
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosAIExplorationWaitingState();

#pragma region PROPERTIES

public:

protected:

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
	
protected:

#pragma endregion
	
};
