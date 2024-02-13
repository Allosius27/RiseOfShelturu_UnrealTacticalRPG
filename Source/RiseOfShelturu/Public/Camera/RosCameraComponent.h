// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

#include "RosCameraComponent.generated.h"

class UCanvas;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

/**
 * URosCameraComponent
 *
 *	The base camera component class used by this project.
 */
UCLASS()
class RISEOFSHELTURU_API URosCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:

	URosCameraComponent(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

protected:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma  region GETTERS

		// Returns the target actor that the camera is looking at.
		virtual AActor* GetTargetActor() const { return GetOwner(); }

	#pragma endregion

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "CameraComponent")
	static URosCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URosCameraComponent>() : nullptr); }

	virtual void DrawDebug(UCanvas* Canvas) const;
	
protected:

#pragma endregion
};
