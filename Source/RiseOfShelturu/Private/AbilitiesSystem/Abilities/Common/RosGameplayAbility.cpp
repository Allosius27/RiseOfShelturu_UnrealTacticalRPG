// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Abilities/Common/RosGameplayAbility.h"

#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "System/Player/RosPlayerController.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Components/Heroes/RosHeroComponent.h"

URosGameplayAbility::URosGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
}

URosAbilitySystemComponent* URosGameplayAbility::GetRosAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<URosAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ARosPlayerController* URosGameplayAbility::GetRosPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ARosPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* URosGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ARosUnit* URosGameplayAbility::GetRosUnitFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ARosUnit>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

URosHeroComponent* URosGameplayAbility::GetHeroComponentFromActorInfo() const
{
	return (CurrentActorInfo ? URosHeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void URosGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void URosGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void URosGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URosGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
