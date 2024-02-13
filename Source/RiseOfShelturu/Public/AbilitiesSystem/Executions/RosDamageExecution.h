// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "RosDamageExecution.generated.h"

class UObject;

struct FDamageAttrStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FDamageAttrStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URosBaseStatsSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URosBaseStatsSet, Damage, Source, true);
	}
};

/**
 * URosDamageExecution
 *
 *	Execution used by gameplay effects to apply damage to the health attributes.
 */
UCLASS()
class RISEOFSHELTURU_API URosDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

public:

	URosDamageExecution();

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
	

#pragma endregion
};
