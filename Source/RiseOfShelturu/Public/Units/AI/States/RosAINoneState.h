// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/AI/States/RosAIBaseState.h"
#include "RosAINoneState.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosAINoneState : public URosAIBaseState
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosAINoneState();

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
