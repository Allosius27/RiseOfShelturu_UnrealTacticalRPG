#pragma once

//#include "Attributes/RosCombatSet.h"

class AActor;
struct FGameplayEffectSpec;

// struct FHealStatics
// {
// 	FGameplayEffectAttributeCaptureDefinition BaseHealDef;
//
// 	FHealStatics()
// 	{
// 		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(URosCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
// 	}
// };
//
// struct FDamageStatics
// {
// 	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
//
// 	FDamageStatics()
// 	{
// 		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(URosCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
// 	}
// };

// Delegate used to broadcast attribute events.
DECLARE_MULTICAST_DELEGATE_FourParams(FRosAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);
