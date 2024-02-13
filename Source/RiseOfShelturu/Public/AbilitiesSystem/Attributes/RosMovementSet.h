// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitiesSystem/Attributes/RosAttributeSet.h"
#include "RosMovementSet.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosMovementSet : public URosAttributeSet
{
	GENERATED_BODY()

	public:

	URosMovementSet();

	ATTRIBUTE_ACCESSORS(URosMovementSet, Speed);

#pragma region PROPERTIES

	public:

	protected:

	private:

		// The current health attribute.  The health will be capped by the max health attribute.
		UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, Category = "MovementSet|Speed", Meta = (AllowPrivateAccess = true))
		FGameplayAttributeData Speed;

#pragma endregion

#pragma region EVENTS

	public:

#pragma endregion

#pragma region FUNCTIONS

	public:

	#pragma  region GETTERS

	#pragma endregion 
	
	protected:

		UFUNCTION()
		void OnRep_Speed(const FGameplayAttributeData& OldValue);

		virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
		virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

		virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
		virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
		virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	#pragma endregion
};
