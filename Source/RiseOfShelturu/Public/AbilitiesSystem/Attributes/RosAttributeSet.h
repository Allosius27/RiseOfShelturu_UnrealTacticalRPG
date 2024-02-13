// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "RosAttributeSet.generated.h"

class AActor;
class URosAbilitySystemComponent;
class UObject;
class UWorld;
struct FGameplayEffectSpec;

/**
 * This macro defines a set of helper functions for accessing and initializing attributes.
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health)
 * will create the following functions:
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * URosAttributeSet
 *
 *	Base attribute set class for the project.
 *	To Visualize Attributes Values : Console Command : showdebug abilitysystem
 */
UCLASS()
class RISEOFSHELTURU_API URosAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	URosAttributeSet();

	UWorld* GetWorld() const override;

	URosAbilitySystemComponent* GetRosAbilitySystemComponent() const;
	
};
