// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RosGameplayAbility.generated.h"

class URosHeroComponent;
class ARosUnit;
class ARosPlayerController;
class URosAbilitySystemComponent;
/**
 * URosGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class RISEOFSHELTURU_API URosGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
#pragma region PROPERTIES

public:

	URosGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	URosAbilitySystemComponent* GetRosAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	ARosPlayerController* GetRosPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	ARosUnit* GetRosUnitFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	URosHeroComponent* GetHeroComponentFromActorInfo() const;

#pragma endregion 
	
protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion
	
};
