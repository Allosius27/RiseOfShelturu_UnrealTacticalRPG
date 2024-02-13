#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "RosUnitsTypes.generated.h"

/**
 * ERosDeathState
 *
 *	Defines current state of death.
 */
UENUM(BlueprintType)
enum class ERosDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UENUM(BlueprintType)
enum EAIExplorationCompanionsStates : uint8
{
	E_CompanionsFollowState      UMETA(DisplayName="CompanionsFollowState"),
	E_CompanionsWaitingState      UMETA(DisplayName="CompanionsWaitingState"),
	E_DebugCompanionsState       UMETA(DisplayName="DebugCompanionsState"),
};

UENUM(BlueprintType)
enum class ERosBattleTeam : uint8
{
	E_Neutral      UMETA(DisplayName="Neutral"),
	E_Player      UMETA(DisplayName="Player"),
	E_Ally       UMETA(DisplayName="Ally"),
	E_Enemy       UMETA(DisplayName="Enemy"),
	E_DebugBattleTeam       UMETA(DisplayName="DebugBattleTeam"),
};

USTRUCT(BlueprintType)
struct FRosUnitSpotPoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpotPointPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor DebugColor;

	// this specific constructor syntax is needed, otherwise there will be at least a warning, possibly an error when you package the project
	FRosUnitSpotPoint() :  IsTaken(false), SpotPointPosition(FVector(0,0,0)), Priority(0), DebugColor(FColor(181,0,0))
	{
	}

	static bool PriorityComparator(const FRosUnitSpotPoint& spotPointA, const FRosUnitSpotPoint& spotPointB)
	{
		return spotPointA.Priority > spotPointB.Priority;
	}
};