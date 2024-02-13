// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RosAIStateMachineBrain.generated.h"

class ARosUnit;
class URosAIBaseState;

UENUM(BlueprintType)
enum EAIStates : uint8
{
	E_DefaultState      UMETA(DisplayName="DefaultState"),
	E_NoneState      UMETA(DisplayName="NoneState"),
	E_ExplorationFollowState      UMETA(DisplayName="ExplorationFollowState"),
	E_ExplorationWaitingState      UMETA(DisplayName="ExplorationWaitingState"),
	E_ExplorationBattleDetectState      UMETA(DisplayName="ExplorationBattleDetectState"),
	E_BattleAITurnState      UMETA(DisplayName="BattleAITurnState"),
	E_DebugState       UMETA(DisplayName="DebugState"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISEOFSHELTURU_API URosAIStateMachineBrain : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	URosAIStateMachineBrain();

#pragma region PROPERTIES

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StateMachine|Exploration|BattleDetection")
	TArray<TEnumAsByte<EObjectTypeQuery>> BattleDetectionObjectTypes;

protected:

	UPROPERTY(BlueprintGetter=GetUnitOwner, VisibleAnywhere)
	ARosUnit* UnitOwner = nullptr;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "StateMachine")
	TEnumAsByte<EAIStates> InitialState;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "StateMachine")
	TMap<TEnumAsByte<EAIStates>, TSubclassOf<URosAIBaseState>> StatesInventory;
	

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "StateMachine")
	URosAIBaseState* CurrentState = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "StateMachine")
	TMap<TEnumAsByte<EAIStates>, URosAIBaseState*> InitialisedStates{};


	UPROPERTY(BlueprintGetter=GetExplorationFollowWaitingTime, EditAnywhere, Category = "StateMachine|Exploration|Follow")
	float ExplorationFollowWaitingTime = 1.0f;

	UPROPERTY(BlueprintGetter=GetExplorationFollowMinimalDistanceToMove, EditAnywhere, Category = "StateMachine|Exploration|Follow")
	float ExplorationFollowMinimalDistanceToMove = 50.0f;

	
	UPROPERTY(BlueprintGetter=GetBattleDetectionRange, EditAnywhere, Category = "StateMachine|Exploration|BattleDetection")
	float BattleDetectionRange = 1000.0f;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable)
	ARosUnit* GetUnitOwner() const { return UnitOwner; }

	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "StateMachine|Exploration|Follow")
	float GetExplorationFollowWaitingTime() const { return ExplorationFollowWaitingTime; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "StateMachine|Exploration|Follow")
	float GetExplorationFollowMinimalDistanceToMove() const { return ExplorationFollowMinimalDistanceToMove; }


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "StateMachine|Exploration|BattleDetection")
	float GetBattleDetectionRange() const { return BattleDetectionRange; }

#pragma endregion

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void ChangeState(TEnumAsByte<EAIStates> NewState);
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void InitialiseStates();

#pragma endregion
};
