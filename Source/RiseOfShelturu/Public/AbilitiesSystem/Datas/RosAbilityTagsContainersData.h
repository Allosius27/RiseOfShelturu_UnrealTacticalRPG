// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RosAbilityTagsContainersData.generated.h"

/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API URosAbilityTagsContainersData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URosAbilityTagsContainersData(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, Category = "TagsContainersData|Abilities")
	FGameplayTagContainer JumpAbilityTagContainer;

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
