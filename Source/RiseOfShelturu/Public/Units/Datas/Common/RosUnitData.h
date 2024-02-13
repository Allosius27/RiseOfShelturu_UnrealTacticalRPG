// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitiesSystem/Datas/RosAbilityTagsContainersData.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "RosUnitData.generated.h"

enum class ERosBattleTeam : uint8;
class URosAbilityTagsContainersData;
class URosAbilitySetData;
class APawn;
class UObject;
class URosInputConfigData;

/**
 * URosUnitData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Ros Unit Data", ShortTooltip = "Data asset used to define a Unit."))
class RISEOFSHELTURU_API URosUnitData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URosUnitData(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

	// Class to instantiate for this pawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData|Pawn")
	TSubclassOf<APawn> PawnClass;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnitData|Attributes")
	UDataTable* AttrMovementDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnitData|Attributes")
	UDataTable* AttrBaseStatsDataTable;

	
	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData|Abilities")
	TArray<TObjectPtr<URosAbilitySetData>> ActiveAbilitySets;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData|Abilities")
	TArray<TObjectPtr<URosAbilitySetData>> PassiveAbilitySets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnitData|Abilities")
	TObjectPtr<URosAbilityTagsContainersData> AbilityTagsContainers;

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter=GetHealth, Category = "UnitData|Attributes|Stats")
	float Health = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetDamages, Category = "UnitData|Attributes|Stats")
	float Damages = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetHealing, Category = "UnitData|Attributes|Stats")
	float Healing = 2.5f;

	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetMovementPoints, Category = "UnitData|Attributes|Tactic", meta=(ClampMin=0, ClampMax=20))
	int MovementPoints = 7;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetActionPoints, Category = "UnitData|Attributes|Tactic", meta=(ClampMin=0, ClampMax=10))
	int ActionPoints = 7;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitiative, Category = "UnitData|Attributes|Tactic", meta=(ClampMin=0, ClampMax=100))
	int Initiative = 10;

	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetWorldSpeed, Category = "UnitData|Attributes|Movement")
	float WorldSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetTacticSpeed, Category = "UnitData|Attributes|Movement")
	float TacticSpeed = 600.0f;

	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetBattleTeam, Category = "UnitData|Team", meta = (DisplayPriority = 0))
	ERosBattleTeam BattleTeam;


#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Stats")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Stats")
	float GetDamages() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Stats")
	float GetHealing() const;


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Tactic")
	int GetMovementPoints() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Tactic")
	int GetActionPoints() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes|Tactic")
	int GetInitiative() const;

	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes")
	float GetWorldSpeed() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Attributes")
	float GetTacticSpeed() const;


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "UnitData|Team")
	ERosBattleTeam GetBattleTeam() const;

#pragma endregion
	
protected:

#pragma endregion
};
