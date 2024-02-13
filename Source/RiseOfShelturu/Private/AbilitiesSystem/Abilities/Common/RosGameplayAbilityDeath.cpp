// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Abilities/Common/RosGameplayAbilityDeath.h"

#include "System/GameInstances/RosGameManager.h"
#include "Units/Components/Common/RosStatsUnitComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosGameplayAbilityDeath)

URosGameplayAbilityDeath::URosGameplayAbilityDeath(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void URosGameplayAbilityDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URosGameplayAbilityDeath::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URosGameplayAbilityDeath::StartDeath()
{
	if (URosStatsUnitComponent* StatsComponent = URosStatsUnitComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (StatsComponent->GetDeathState() == ERosDeathState::NotDead)
		{
			UE_LOG(LogRos, Warning, TEXT("Start Death Ability"));

			StatsComponent->StartDeath();
		}
	}
}

void URosGameplayAbilityDeath::FinishDeath()
{
	if (URosStatsUnitComponent* StatsComponent = URosStatsUnitComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (StatsComponent->GetDeathState() == ERosDeathState::DeathStarted)
		{
			UE_LOG(LogRos, Warning, TEXT("Finish Death Ability"));

			StatsComponent->FinishDeath();
		}
	}
}
