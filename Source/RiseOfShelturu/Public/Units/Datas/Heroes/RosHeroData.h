// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Datas/Common/RosUnitData.h"
#include "RosHeroData.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosHeroData : public URosUnitData
{
	GENERATED_BODY()

public:

	URosHeroData(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:
	
	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HeroData|Input")
	TObjectPtr<URosInputConfigData> InputConfig;

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomInterpolation, Category = "HeroData|Zoom")
	float ZoomInterpolation = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomSpeed, Category = "HeroData|Zoom")
	float ZoomSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomMin, Category = "HeroData|Zoom")
	float ZoomMin = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetZoomMax, Category = "HeroData|Zoom")
	float ZoomMax = 800.0f;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroData|Zoom")
	float GetZoomInterpolation() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroData|Zoom")
	float GetZoomSpeed() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroData|Zoom")
	float GetZoomMin() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroData|Zoom")
	float GetZoomMax() const;

#pragma endregion 
	
protected:

#pragma endregion
};
