// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Attributes/RosAttributeSet.h"

#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosAttributeSet)

class UWorld;

URosAttributeSet::URosAttributeSet()
{
}

UWorld* URosAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

URosAbilitySystemComponent* URosAttributeSet::GetRosAbilitySystemComponent() const
{
	return Cast<URosAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
