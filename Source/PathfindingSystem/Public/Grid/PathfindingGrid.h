// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PathfindingCellClass.h"
#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "PathfindingSystemTypes.h"
#include "PathfindingGrid.generated.h"

// forward declaration
class ATestUnit;			
class UPathfindingMoveComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateGridSignature, bool, SnapUnitOwner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetPathSignature, TArray<FVector2D>, path);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FApplyActionOnWorldCellPositionSignature, FVector, worldLocation, FVector2D, cellGridPos);

UCLASS()
class PATHFINDINGSYSTEM_API APathfindingGrid : public AActor
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	APathfindingGrid();

#pragma region PROPERTIES

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 0))
	TEnumAsByte<EPathfindingType> PathfindingType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 0, ToolTip = "The scale of the flow field pathfinding boundary in units"))
	FVector2D GridSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 1, ToolTip = "The scale of the cells in units"))
	float CellSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 3, ToolTip = "If True: Shows the debug grid in-game"))
	bool bShowGridInGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 4, ToolTip = "If True: Shows the flow field arrows in-game"))
	bool bShowArrowsInGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 5, ToolTip = "If True: When creating the grid it will sphere-trace for any mesh that isnt of the given Ground Object Type"))
	bool bTraceForObstacles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 6, ToolTip = "Type of object the pathfinding deem as an obstacle"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleType;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 7, ToolTip = "If True: Trace all cells for any given Ground Object Type and aligns it to the ground"))
	bool bAlignToGround;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 8, ToolTip = "The height of which the ground trace will start at, in units"))
	float TraceHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 9, ToolTip = "The max walkable ground angle of which the ground trace will deem as a walkable cell"))
	float MaxWalkableGroundAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 10, ToolTip = "Object types that the system will align to"))
	TArray<TEnumAsByte<EObjectTypeQuery>> GroundObjectType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 11, ToolTip = "Type of object the pathfinding deem as an difficult ground"))
	TArray<TEnumAsByte<EObjectTypeQuery>> DifficultGroundType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathfindingGrid", meta = (DisplayPriority = 12, ToolTip = "Type of object the pathfinding deem as an really difficult ground"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ReallyDifficultGroundType;

	
	UPROPERTY(BlueprintReadWrite, Category = "PathfindingGrid")
	float CellRadius;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid")
	TArray<FS_Cell> AllGridCells;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|AStar")
	TMap<FVector2D, FS_Cell> CellsMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|AStar")
	TArray<FVector2D> Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|AStar")
	bool bAUnitMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|AStar")
	FVector2D CurrentOveredCellGridPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|AStar")
	FVector2D CurrentSelectedCellGridPos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingGrid|FlowField")
	TMap<FVector2D, FVector> DirMap;

	
	
	UPROPERTY(meta = (DisplayPriority = 2))
	FVector2D GoalPosition;
	
	UPROPERTY()
	int XAmount;

	UPROPERTY()
	int YAmount;
	
	UPROPERTY()
	TArray<FS_Cell> AllCellsBP;
	
	TArray<PathfindingCellClass> AllCells;

	PathfindingCellClass DestinationCell;


	UPROPERTY()
	UBoxComponent* BoundsBox;
	
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PathfindingGrid|Debug")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponentArrow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PathfindingGrid|Debug")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponentCell;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PathfindingGrid|Debug")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponentCellDifficult;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PathfindingGrid|Debug")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponentCellReallyDifficult;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PathfindingGrid|Debug")
	UInstancedStaticMeshComponent* InstancedStaticMeshComponentObstacle;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPathfindingMoveComponent* SelectedUnitMoveComponent;

#pragma endregion

#pragma region EVENTS

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGetPathSignature OnGetGridPath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FApplyActionOnWorldCellPositionSignature OnSelectNewCell;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FApplyActionOnWorldCellPositionSignature OnHighlightCell;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateGridSignature OnPreResetGrid;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateGridSignature OnPostResetGrid;

#pragma endregion

#pragma region FUNCTIONS
	
public:
	
	/*Construction script*/
	virtual void OnConstruction(const FTransform& scale) override;

	
	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid", meta = (ToolTip = "Creates the grid based on the parameters, outputs an array that contains all the grid cells."))
	void CreateGrid(TArray<FS_Cell>& GridCells);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "PathfindingGrid", meta = (ToolTip = "Creates a debug grid."))
	void DebugGrid();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PathfindingGrid", meta = (ToolTip = "Reset Grid Parameters"))
	void ResetGrid(bool snapUnitOwner = true);
	
	
	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|FlowField", meta = (ToolTip = "Generates a flow field based on the input Grid Cells Array and an estimate goal World Position, outputs a Map which contains all the cell directions based on a 2D position and an exact goal world position."))
	void GenerateFlowField(const TArray<FS_Cell> GridCells, const FVector goal, TMap<FVector2D, FVector>& directionMap, FVector& goalWorldPos);
	
	virtual TArray<PathfindingCellClass> CreateIntegrationField(const FVector2D destinationPos, const TArray<PathfindingCellClass> gridCells);

	virtual TArray<PathfindingCellClass> CreateFlowField(const TArray<PathfindingCellClass> gridCells);

	
	virtual bool IsDiagonalValid(const int loopIndex, const int curIndex, const int neighbourIndex);

	
	UFUNCTION()
	virtual bool IfNeigbourClose(const FVector2D currentPos, const FVector2D neighbourPos);
	
	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	int GetCellCost(TEnumAsByte<EGroundType> gridIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PathfindingGrid|AStar")
	TArray<FVector2D> GetCellNeighbours(FVector2D gridIndex, bool displayDebug = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PathfindingGrid|AStar")
	FVector2D GetClosestCellNeighbour(FVector2D gridIndexTarget, FVector2D gridIndexSource,bool& successCellFound, bool displayDebug = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PathfindingGrid|AStar")
	int GetEstimatedCostToTarget(FVector2D currentCell, FVector2D targetCell);
	

	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	void ResetPath();
	
	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	void FindPathToTarget(FVector2D startCell, FVector2D targetCell);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PathfindingGrid|AStar")
	TArray<FVector2D> RetracePath(FVector2D targetCell, FVector2D startCell);

	
	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	void SetSelectedUnitComponent(UPathfindingMoveComponent* pathfindingUnitMoveComponent);


	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	void OverNewCell(FVector2D overedCellGridPos);

	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|AStar")
	void SelectNewCell(FVector2D selectedCellGridPos);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PathfindingGrid|AStar")
	void OnHighlightCurrentPath(bool isForHighlight);


	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid|Utility")
	static FVector2D FindGridCell(FVector worldPos, APathfindingGrid* grid);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "PathfindingGrid")
	void Start();

	// Called every frame
	virtual void Tick(float deltaTime) override;

#pragma endregion 
};
