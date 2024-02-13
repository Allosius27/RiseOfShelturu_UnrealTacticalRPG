// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RosAnimInstance.h"

#include "Units/Actors/Common/RosUnit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosAnimInstance)

URosAnimInstance::URosAnimInstance()
{
	
}

#if WITH_EDITOR
EDataValidationResult URosAnimInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	return Super::IsDataValid(ValidationErrors);
}
#endif

void URosAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void URosAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ARosUnit* Character = Cast<ARosUnit>(GetOwningActor());
	if (!Character)
	{
		return;
	}
}
