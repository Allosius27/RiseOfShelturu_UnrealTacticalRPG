// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RosAnimInstance.generated.h"

/**
 * URosAnimInstance
 *
 *	The base game animation instance class used by this project.
 */

UCLASS(Config = Game)
class RISEOFSHELTURU_API URosAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	URosAnimInstance();

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

#if WITH_EDITOR
	
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;

#endif // WITH_EDITOR

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma endregion
};
