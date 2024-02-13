// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Abilities/Common/RosGameplayAbilitySpeedPassive.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/GameInstances/RosGameManager.h"
#include "Units/Actors/Common/RosUnit.h"

URosGameplayAbilitySpeedPassive::URosGameplayAbilitySpeedPassive(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URosGameplayAbilitySpeedPassive::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URosGameplayAbilitySpeedPassive::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URosGameplayAbilitySpeedPassive::ApplyAttributeSpeedChange()
{
	ARosUnit* rosUnit = GetRosUnitFromActorInfo();
	if(rosUnit != nullptr)
	{
		bool bSuccessfullyFoundAttribute = false;
		float floatSpeedAttributeValue = UAbilitySystemBlueprintLibrary::GetFloatAttributeFromAbilitySystemComponent(rosUnit->GetAbilitySystemComponent(),
			AttrSpeed, bSuccessfullyFoundAttribute);
		if(bSuccessfullyFoundAttribute)
		{
			rosUnit->GetCharacterMovement()->MaxWalkSpeed = floatSpeedAttributeValue;
		}
		else
		{
			UE_LOG(LogRos, Error, TEXT("Failed to find speed attribute"));
		}
		OnResetWaitingAttributeSpeedChange();
	}
}
