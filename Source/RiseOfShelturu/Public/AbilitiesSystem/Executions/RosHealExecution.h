// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "RosHealExecution.generated.h"

class UObject;

struct FHealAttrStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Healing);

	FHealAttrStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URosBaseStatsSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URosBaseStatsSet, Healing, Source, true);
	}
};

/**
 * URosHealExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 */
UCLASS()
class RISEOFSHELTURU_API URosHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	URosHealExecution();

#pragma region PROPERTIES

public:

protected:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion 
	
protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

#pragma endregion
};
