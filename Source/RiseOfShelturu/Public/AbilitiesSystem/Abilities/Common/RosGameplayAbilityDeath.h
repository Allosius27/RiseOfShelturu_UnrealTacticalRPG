// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesSystem/Abilities/Common/RosGameplayAbility.h"
#include "RosGameplayAbilityDeath.generated.h"

/**
 * URosGameplayAbility_Death
 *
 *	Gameplay ability used for handling death.
 *	Ability is activated automatically via the "GameplayEvent.Death" ability trigger tag.
 */
UCLASS(Abstract)
class RISEOFSHELTURU_API URosGameplayAbilityDeath : public URosGameplayAbility
{
	GENERATED_BODY()

public:

	URosGameplayAbilityDeath(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Starts the death sequence.
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Death")
	void StartDeath();

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Death")
	void FinishDeath();

#pragma endregion
};
