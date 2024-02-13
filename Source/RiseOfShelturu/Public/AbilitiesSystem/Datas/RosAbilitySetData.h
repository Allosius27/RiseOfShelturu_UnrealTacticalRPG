// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpecHandle.h"

#include "RosAbilitySetData.generated.h"

class UAttributeSet;
class UGameplayEffect;
class URosAbilitySystemComponent;
class URosGameplayAbility;
class UObject;

/**
 * FRosAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FRosAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URosGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * FRosAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FRosAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

// /**
//  * FRosAbilitySet_AttributeSet
//  *
//  *	Data used by the ability set to grant attribute sets.
//  */
// USTRUCT(BlueprintType)
// struct FRosAbilitySet_AttributeSet
// {
// 	GENERATED_BODY()
//
// public:
// 	// Gameplay effect to grant.
// 	UPROPERTY(EditDefaultsOnly)
// 	TSubclassOf<UAttributeSet> AttributeSet;
//
// };

/**
 * FRosAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FRosAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// // Pointers to the granted attribute sets
	// UPROPERTY()
	// TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
	
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	//void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(URosAbilitySystemComponent* RosASC);

};

/**
 * URosAbilitySet
 *
 *	Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class RISEOFSHELTURU_API URosAbilitySetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URosAbilitySetData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

protected:

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySetData|GameplayAbilities", meta=(TitleProperty=Ability))
	TArray<FRosAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySetData|GameplayEffects", meta=(TitleProperty=GameplayEffect))
	TArray<FRosAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	// UPROPERTY(EditDefaultsOnly, Category = "AbilitySetData|AttributeSets", meta=(TitleProperty=AttributeSet))
	// TArray<FRosAbilitySet_AttributeSet> GrantedAttributes;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(URosAbilitySystemComponent* RosASC, FRosAbilitySet_GrantedHandles* OutGrantedHandles, bool IsPassiveAbilities = false, UObject* SourceObject = nullptr) const;
	
protected:

#pragma endregion
};
