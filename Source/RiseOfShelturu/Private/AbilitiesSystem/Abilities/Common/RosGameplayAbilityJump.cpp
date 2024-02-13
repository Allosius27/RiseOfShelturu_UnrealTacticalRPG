// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Abilities/Common/RosGameplayAbilityJump.h"

#include "Units/Actors/Common/RosUnit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosGameplayAbilityJump)

URosGameplayAbilityJump::URosGameplayAbilityJump(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool URosGameplayAbilityJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ARosUnit* RosUnit = Cast<ARosUnit>(ActorInfo->AvatarActor.Get());
	if (!RosUnit || !RosUnit->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void URosGameplayAbilityJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	//CharacterJumpStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URosGameplayAbilityJump::CharacterJumpStart()
{
	if (ARosUnit* RosUnit = GetRosUnitFromActorInfo())
	{
		RosUnit->UnCrouch();
		RosUnit->Jump();
	}
}

void URosGameplayAbilityJump::CharacterJumpStop()
{
	if (ARosUnit* RosUnit = GetRosUnitFromActorInfo())
	{
		RosUnit->StopJumping();
	}
}
