// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "RiseOfShelturu/RosGameplayTags.h"
#include "Units/Components/Common/RosStatsUnitComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosBaseStatsSet)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageSelfDestruct, "Gameplay.Damage.SelfDestruct");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_FellOutOfWorld, "Gameplay.Damage.FellOutOfWorld");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ros_Damage_Message, "Ros.Damage.Message");

URosBaseStatsSet::URosBaseStatsSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
	, Healing(10.0f)
	, Damage(10.0f)
	, Initiative(10)
	, MaxInitiative(10)
	, MovementPoints(7)
	, MaxMovementPoints(7)
	, ActionPoints(7)
	, MaxActionPoints(7)
{
	bOutOfHealth = false;
}

void URosBaseStatsSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URosBaseStatsSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URosBaseStatsSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void URosBaseStatsSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URosBaseStatsSet, Health, OldValue);
}

void URosBaseStatsSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URosBaseStatsSet, MaxHealth, OldValue);
}

bool URosBaseStatsSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Handle modifying incoming normal damage
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);

			if (Data.Target.HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity) && !bIsDamageFromSelfDestruct)
			{
				// Do not take away any health.
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}

#if !UE_BUILD_SHIPPING
			// Check GodMode cheat, unlimited health is checked below
			if (Data.Target.HasMatchingGameplayTag(RosGameplayTags::Cheat_GodMode) && !bIsDamageFromSelfDestruct)
			{
				// Do not take away any health.
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
#endif // #if !UE_BUILD_SHIPPING
		}
	}

	return true;
}

void URosBaseStatsSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);
	float MinimumHealth = 0.0f;

#if !UE_BUILD_SHIPPING
	// Godmode and unlimited health stop death unless it's a self destruct
	if (!bIsDamageFromSelfDestruct &&
		(Data.Target.HasMatchingGameplayTag(RosGameplayTags::Cheat_GodMode) || Data.Target.HasMatchingGameplayTag(RosGameplayTags::Cheat_UnlimitedHealth) ))
	{
		MinimumHealth = 1.0f;
	}
#endif // #if !UE_BUILD_SHIPPING

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Send a standardized verb message that other systems can observe
		//if (Data.EvaluatedData.Magnitude > 0.0f)
		//{
		//}

		// Convert into -Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp and fall into out of health handling below
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetInitiativeAttribute())
	{
		SetInitiative(FMath::Clamp(GetInitiative(), 1.0f, GetMaxInitiative()));
	}
	else if (Data.EvaluatedData.Attribute == GetMovementPointsAttribute())
	{
		SetMovementPoints(FMath::Clamp(GetMovementPoints(), 0.0f, GetMaxMovementPoints()));
	}
	else if (Data.EvaluatedData.Attribute == GetActionPointsAttribute())
	{
		SetActionPoints(FMath::Clamp(GetActionPoints(), 0.0f, GetMaxActionPoints()));
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		if (OnOutOfHealth.IsBound())
		{
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
			AActor* Instigator = EffectContext.GetOriginalInstigator();
			AActor* Causer = EffectContext.GetEffectCauser();

			OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
		}
	}

	// Check health again in case an event above changed it.
	bOutOfHealth = (GetHealth() <= 0.0f);
}

void URosBaseStatsSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void URosBaseStatsSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void URosBaseStatsSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent();
			check(RosASC);

			RosASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (Attribute == GetMaxInitiativeAttribute())
	{
		if (GetInitiative() > NewValue)
		{
			URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent();
			check(RosASC);

			RosASC->ApplyModToAttribute(GetInitiativeAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (Attribute == GetMaxMovementPointsAttribute())
	{
		if (GetMovementPoints() > NewValue)
		{
			URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent();
			check(RosASC);

			RosASC->ApplyModToAttribute(GetMovementPointsAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (Attribute == GetMaxActionPointsAttribute())
	{
		if (GetActionPoints() > NewValue)
		{
			URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent();
			check(RosASC);

			RosASC->ApplyModToAttribute(GetActionPointsAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}

void URosBaseStatsSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}

	if (Attribute == GetInitiativeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxInitiative());
	}
	else if (Attribute == GetMaxInitiativeAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}

	if (Attribute == GetMovementPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMovementPoints());
	}
	else if (Attribute == GetMaxMovementPointsAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}

	if (Attribute == GetActionPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxActionPoints());
	}
	else if (Attribute == GetMaxActionPointsAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
