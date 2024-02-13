// Fill out your copyright notice in the Description page of Project Settings.

#include "Inputs/Datas/RosInputConfigData.h"

#include "Inputs/RosInputsTypes.h"
#include "System/GameInstances/RosGameManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosInputConfigData)

URosInputConfigData::URosInputConfigData(const FObjectInitializer& ObjectInitializer)
{
	
}

const UInputAction* URosInputConfigData::FindNativeInputActionForTag(const FGameplayTag& InputTag,
                                                                     bool bLogNotFound) const
{
	for (const FRosInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogRos, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* URosInputConfigData::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FRosInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogRos, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
