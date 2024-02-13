// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesSystem/Abilities/Common/RosGameplayAbility.h"
#include "RosGameplayAbilityJump.generated.h"

/**
 * URosGameplayAbility_Jump
 *
 *	Gameplay ability used for character jumping.
 */
UCLASS(Abstract)
class RISEOFSHELTURU_API URosGameplayAbilityJump : public URosGameplayAbility
{
	GENERATED_BODY()

public:

	URosGameplayAbilityJump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Jump")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Jump")
	void CharacterJumpStop();

#pragma endregion
};
