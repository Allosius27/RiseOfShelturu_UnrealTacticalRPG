// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Datas/RosAbilitySetData.h"

#include "AbilitiesSystem/Abilities/Common/RosGameplayAbility.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "System/GameInstances/RosGameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosAbilitySetData)

void FRosAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FRosAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

// void FRosAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
// {
// 	GrantedAttributeSets.Add(Set);
// }

void FRosAbilitySet_GrantedHandles::TakeFromAbilitySystem(URosAbilitySystemComponent* RosASC)
{
	check(RosASC);

	if (!RosASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			RosASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			RosASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	// for (UAttributeSet* Set : GrantedAttributeSets)
	// {
	// 	RosASC->RemoveSpawnedAttribute(Set);
	// }

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	//GrantedAttributeSets.Reset();
}

URosAbilitySetData::URosAbilitySetData(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void URosAbilitySetData::GiveToAbilitySystem(URosAbilitySystemComponent* RosASC,
	FRosAbilitySet_GrantedHandles* OutGrantedHandles, bool IsPassiveAbilities, UObject* SourceObject) const
{
	check(RosASC);

	if (!RosASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FRosAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogRos, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		URosGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<URosGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = RosASC->GiveAbility(AbilitySpec);
		if(IsPassiveAbilities)
		{
			RosASC->CallServerTryActivateAbility(AbilitySpecHandle, false, FPredictionKey());
		}
		

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FRosAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogRos, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = RosASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, RosASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// // Grant the attribute sets.
	// for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	// {
	// 	const FRosAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];
	//
	// 	if (!IsValid(SetToGrant.AttributeSet))
	// 	{
	// 		UE_LOG(LogRos, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
	// 		continue;
	// 	}
	//
	// 	UAttributeSet* NewSet = NewObject<UAttributeSet>(RosASC->GetOwner(), SetToGrant.AttributeSet);
	// 	RosASC->AddAttributeSetSubobject(NewSet);
	//
	// 	if (OutGrantedHandles)
	// 	{
	// 		OutGrantedHandles->AddAttributeSet(NewSet);
	// 	}
	// }
}
