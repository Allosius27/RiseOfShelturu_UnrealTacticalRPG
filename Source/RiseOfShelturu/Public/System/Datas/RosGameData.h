// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RosGameData.generated.h"

class URosUnitData;
/**
 * URosGameData
 *
 *	Non-mutable data asset that contains global game data.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Ros Game Data", ShortTooltip = "Data asset containing global game data."))
class RISEOFSHELTURU_API URosGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URosGameData();

#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, Category = "GameData|Player")
	TArray<TObjectPtr<const URosUnitData>> PlayerTeamUnitsDatas;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion 
	
protected:

#pragma endregion
};
