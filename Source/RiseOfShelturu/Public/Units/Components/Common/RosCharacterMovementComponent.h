// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RosCharacterMovementComponent.generated.h"

/**
 * URosCharacterMovementComponent
 *
 *	The base character movement component class used by this project.
 */
UCLASS(Config = Game)
class RISEOFSHELTURU_API URosCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	URosCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion
	
	virtual void SimulateMovement(float DeltaTime) override;

	virtual bool CanAttemptJump() const override;

	//~UMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface
	
protected:

	virtual void InitializeComponent() override;

#pragma endregion
};
