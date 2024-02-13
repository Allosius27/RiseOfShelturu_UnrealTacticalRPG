// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Components/Common/RosCharacterMovementComponent.h"

URosCharacterMovementComponent::URosCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void URosCharacterMovementComponent::SimulateMovement(float DeltaTime)
{
	Super::SimulateMovement(DeltaTime);
}

bool URosCharacterMovementComponent::CanAttemptJump() const
{
	// Same as UCharacterMovementComponent's implementation but without the crouch check
	return IsJumpAllowed() &&
		(IsMovingOnGround() || IsFalling()); // Falling included for double-jump and non-zero jump hold time, but validated by character.
}

void URosCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

FRotator URosCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	return Super::GetDeltaRotation(DeltaTime);
}

float URosCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed();
}


