// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Executions/RosHealExecution.h"

#include "AbilitiesSystem/RosAbilitiesSystemTypes.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "System/GameInstances/RosGameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosHealExecution)

// static FHealStatics& HealStatics()
// {
// 	static FHealStatics Statics;
// 	return Statics;
// }

URosHealExecution::URosHealExecution()
{
	FHealAttrStruct attributes;

	RelevantAttributesToCapture.Add(attributes.HealthDef);
	//InvalidScopedModifierAttributes.Add(attributes.HealthDef);

	RelevantAttributesToCapture.Add(attributes.HealingDef);
}

void URosHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogRos, Warning, TEXT("Ros Heal Execution"));
	
	FHealAttrStruct Attributes;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvaluationParameters, health);

	float healing = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealingDef, EvaluationParameters, healing);

	float healingDone = healing;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attributes.HealthProperty, EGameplayModOp::Additive, healingDone));
}
