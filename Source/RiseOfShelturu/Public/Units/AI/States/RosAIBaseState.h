// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RosAIBaseState.generated.h"

class ARosUnit;
class ARosPlayerController;
class URosAIStateMachineBrain;
/**
 * 
 */
UCLASS(Abstract)
class RISEOFSHELTURU_API URosAIBaseState : public UObject
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URosAIBaseState();

#pragma region PROPERTIES

public:

protected:

	UPROPERTY(BlueprintGetter=GetStateMachineBrainOwner)
	URosAIStateMachineBrain* StateMachineBrainOwner = nullptr;

	UPROPERTY(BlueprintGetter=GetActorOwner)
	AActor* ActorOwner = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ARosUnit* UnitOwner;

	UPROPERTY(BlueprintReadOnly)
	ARosPlayerController* CurrentPlayerController;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable)
	URosAIStateMachineBrain* GetStateMachineBrainOwner() const { return StateMachineBrainOwner; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	AActor* GetActorOwner() const { return ActorOwner; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	ARosUnit* GetUnitOwner() const { return UnitOwner; }

#pragma endregion

	virtual void OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner);

	virtual void OnEnter();

	virtual void OnExit();

	virtual void OnTick(float DeltaTime);
	
protected:

#pragma endregion
};
