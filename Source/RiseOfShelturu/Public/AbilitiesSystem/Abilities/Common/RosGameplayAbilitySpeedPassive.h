// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesSystem/Abilities/Common/RosGameplayAbility.h"
#include "RosGameplayAbilitySpeedPassive.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosGameplayAbilitySpeedPassive : public URosGameplayAbility
{
	GENERATED_BODY()

public:
	URosGameplayAbilitySpeedPassive(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

protected:

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GameplayAbility|SpeedPassive")
	FGameplayAttribute AttrSpeed;

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

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|SpeedPassive")
	void ApplyAttributeSpeedChange();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameplayAbility|SpeedPassive")
	void OnResetWaitingAttributeSpeedChange();

#pragma endregion
};
