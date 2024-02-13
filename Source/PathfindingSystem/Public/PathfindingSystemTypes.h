#pragma once
#include "GameplayTagContainer.h"
#include "Grid/PathfindingMoveComponent.h"
#include "UObject/ObjectMacros.h"
#include "CoreMinimal.h"
#include "NiagaraComponent.h"

#include "PathfindingSystemTypes.generated.h"

class UNiagaraComponent;

/**
 * 
 * EPathfindingType
 *	
 */
UENUM(BlueprintType)
enum EPathfindingType : uint8 {
	E_AStar       UMETA(DisplayName="AStar"),
	E_FlowField        UMETA(DisplayName="FlowField"),
};

/**
 * 
 * EGroundType
 *	
 */
UENUM(BlueprintType)
enum EGroundType : uint8 {
	E_Normal       UMETA(DisplayName="Normal"),
	E_Difficult        UMETA(DisplayName="Difficult"),
	E_ReallyDifficult       UMETA(DisplayName="ReallyDifficult"),
	E_Impossible       UMETA(DisplayName="Impossible"),
	E_None      UMETA(DisplayName="None"),
	E_DebugGroundType      UMETA(DisplayName="DebugGroundType"),
};

/**
 * FS_Cell
 *
 */
USTRUCT(BlueprintType)
struct FS_Cell
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding")
	UPathfindingMoveComponent* PathfindingUnitComponentRef = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Pathfinding")
	UNiagaraComponent* TestHighlightFxInstanceRef = nullptr;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding")
	TEnumAsByte<EGroundType> GroundType = E_None;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding")
	FVector2D CellGridPos = FVector2d(0.0f, 0.0f);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding")
	FVector CellWorldPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding")
	int CellCost = 0;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding|AStar")
	int CellFinalCost = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding|AStar")
	int CellCostFromStart = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding|AStar")
	int CellEstimatedCostToTarget = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Pathfinding|AStar")
	FVector2D PreviousCellGridPos = FVector2d(0.0f, 0.0f);

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pathfinding|FlowField")
	int CellBestCost = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pathfinding|FlowField")
	int CellIndex = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pathfinding|FlowField")
	FVector CellDir = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pathfinding|FlowField")
	FVector CellNormal = FVector(0.0f, 0.0f, 0.0f);
};