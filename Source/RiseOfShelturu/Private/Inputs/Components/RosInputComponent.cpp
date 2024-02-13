// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/Components/RosInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "System/Player/RosLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosInputComponent)

class URosInputConfigData;

URosInputComponent::URosInputComponent()
{
	
}

void URosInputComponent::AddInputMappings(const URosInputConfigData* InputConfig,
                                          UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	URosLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<URosLocalPlayer>();
	check(LocalPlayer);
}

void URosInputComponent::RemoveInputMappings(const URosInputConfigData* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	URosLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<URosLocalPlayer>();
	check(LocalPlayer);
}

void URosInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
