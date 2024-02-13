// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesSystem/Executions/RosDamageExecution.h"

#include "AbilitiesSystem/RosAbilitiesSystemTypes.h"
#include "System/GameInstances/RosGameManager.h"

URosDamageExecution::URosDamageExecution()
{
	FDamageAttrStruct attributes;

	RelevantAttributesToCapture.Add(attributes.HealthDef);
	//InvalidScopedModifierAttributes.Add(attributes.HealthDef);

	RelevantAttributesToCapture.Add(attributes.DamageDef);
}

void URosDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogRos, Warning, TEXT("Ros Damage Execution"));
	
	FDamageAttrStruct Attributes;
	
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

	float damages = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.DamageDef, EvaluationParameters, damages);

	float damagesDone = damages;

	if(health - damagesDone <= 0.0f)
	{
		FGameplayEventData eventData;
		eventData.EventTag = UGameplayTagsManager::Get().RequestGameplayTag("Event.Die");
		eventData.Instigator = SourceActor;
		eventData.Target = TargetActor;
		TargetASC->HandleGameplayEvent(eventData.EventTag, &eventData);
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attributes.HealthProperty, EGameplayModOp::Additive, -damagesDone));
}
