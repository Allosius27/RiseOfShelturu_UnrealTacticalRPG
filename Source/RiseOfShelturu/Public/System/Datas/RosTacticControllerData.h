// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RosTacticControllerData.generated.h"

class URosInputConfigData;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosTacticControllerData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URosTacticControllerData(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TacticControllerData|Input")
	TObjectPtr<URosInputConfigData> InputConfig;

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter=GetLocationInterpolation, Category = "TacticControllerData|Location")
	float LocationInterpolation = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetLocationSpeed, Category = "TacticControllerData|Location")
	float LocationSpeed = 15.0f;


	UPROPERTY(EditAnywhere, BlueprintGetter=GetRotationInterpolation, Category = "TacticControllerData|Rotation")
	float RotationInterpolation = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetRotationSpeed, Category = "TacticControllerData|Rotation")
	float RotationSpeed = 45.0f;


	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomInterpolation, Category = "TacticControllerData|Zoom")
	float ZoomInterpolation = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomSpeed, Category = "TacticControllerData|Zoom")
	float ZoomSpeed = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomMin, Category = "TacticControllerData|Zoom")
	float ZoomMin = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomMax, Category = "TacticControllerData|Zoom")
	float ZoomMax = 5000.0f;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Location")
	float GetLocationInterpolation() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Location")
	float GetLocationSpeed() const;


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Rotation")
	float GetRotationInterpolation() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Rotation")
	float GetRotationSpeed() const;


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Zoom")
	float GetZoomInterpolation() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Zoom")
	float GetZoomSpeed() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Zoom")
	float GetZoomMin() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "TacticControllerData|Zoom")
	float GetZoomMax() const;

#pragma endregion 
	
protected:

#pragma endregion
};
