// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitiesSystem/Attributes/RosAttributeSet.h"
#include "NativeGameplayTags.h"
#include "AbilitiesSystem/RosAbilitiesSystemTypes.h"
#include "RosBaseStatsSet.generated.h"

class UObject;
struct FFrame;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ros_Damage_Message);

struct FGameplayEffectModCallbackData;


/**
 * URosHealthSet
 *
 *	Class that defines attributes that are necessary for taking damage.
 *	Attribute examples include: health, shields, and resistances.
 */
UCLASS(BlueprintType)
class RISEOFSHELTURU_API URosBaseStatsSet : public URosAttributeSet
{
	GENERATED_BODY()

public:

	URosBaseStatsSet();

	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, Health);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, Healing);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, Damage);

	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, Initiative);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, MaxInitiative);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, MovementPoints);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, MaxMovementPoints);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, ActionPoints);
	ATTRIBUTE_ACCESSORS(URosBaseStatsSet, MaxActionPoints);

#pragma region PROPERTIES

public:

	// The current health attribute.  The health will be capped by the max health attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "StatsSet|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "StatsSet|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;


	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Initiative;
	
	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxInitiative;

	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MovementPoints;

	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxMovementPoints;

	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ActionPoints;

	UPROPERTY(BlueprintReadOnly, Category="StatsSet|Battle", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxActionPoints;
	
protected:

private:
	
	// Used to track when the health reaches 0.
	bool bOutOfHealth;

#pragma endregion

#pragma region EVENTS

public:
	// Delegate to broadcast when the health attribute reaches zero.
	mutable FRosAttributeEvent OnOutOfHealth;

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion 
	
protected:

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

#pragma endregion
	
};
