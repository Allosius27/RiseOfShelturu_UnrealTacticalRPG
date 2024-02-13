// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/PathfindingGrid.h"
#include "Grid/PathfindingMoveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APathfindingGrid::APathfindingGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set InstancedStaticMeshComponent, no collision
	InstancedStaticMeshComponentCell = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ALLGRIDS"));
	InstancedStaticMeshComponentCell->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentCell->SetGenerateOverlapEvents(false);
	RootComponent = InstancedStaticMeshComponentCell;

	InstancedStaticMeshComponentArrow = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ALLARROWS"));
	InstancedStaticMeshComponentArrow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentArrow->SetGenerateOverlapEvents(false);
	InstancedStaticMeshComponentArrow->SetupAttachment(RootComponent);

	InstancedStaticMeshComponentObstacle = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ALLOBSTACLES"));
	InstancedStaticMeshComponentObstacle->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentObstacle->SetGenerateOverlapEvents(false);
	InstancedStaticMeshComponentObstacle->SetupAttachment(RootComponent);

	InstancedStaticMeshComponentCellReallyDifficult = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ALLREALLYDIFFICULTGRIDS"));
	InstancedStaticMeshComponentCellReallyDifficult->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentCellReallyDifficult->SetGenerateOverlapEvents(false);
	InstancedStaticMeshComponentCellReallyDifficult->SetupAttachment(RootComponent);

	InstancedStaticMeshComponentCellDifficult = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ALLDIFFICULTGRIDS"));
	InstancedStaticMeshComponentCellDifficult->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	InstancedStaticMeshComponentCellDifficult->SetGenerateOverlapEvents(false);
	InstancedStaticMeshComponentCellDifficult->SetupAttachment(RootComponent);

	BoundsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	BoundsBox->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BoundsBox->SetGenerateOverlapEvents(false);
	BoundsBox->SetupAttachment(RootComponent);
	
	CellSize = float(100);
	CellRadius = float(CellSize / 2);

	GridSize = FVector2D(1000, 1000);

	XAmount = int(0);
	YAmount = int(0);

	bShowGridInGame = true;
	bShowArrowsInGame = true;
	GoalPosition = FVector2D(-1, -1);
	bTraceForObstacles = true;
	TraceHeight = 300;
	bAlignToGround = false;
	MaxWalkableGroundAngle = 44;
	ObstacleType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
}

void APathfindingGrid::OnConstruction(const FTransform& scale)
{
	Super::OnConstruction(scale);

	BoundsBox->SetBoxExtent(FVector(GridSize.X / 2, GridSize.Y / 2, TraceHeight / 2));
	BoundsBox->SetRelativeLocation(FVector(GridSize.X / 2, GridSize.Y / 2, TraceHeight / 2));
}

// Called when the game starts or when spawned
void APathfindingGrid::BeginPlay()
{
	Super::BeginPlay();

	XAmount = int(GridSize.X / CellSize);
	YAmount = int(GridSize.Y / CellSize);
}

void APathfindingGrid::Start()
{
	ResetGrid();
}


// Called every frame
void APathfindingGrid::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void APathfindingGrid::SetSelectedUnitComponent(UPathfindingMoveComponent* pathfindingUnitMoveComponent)
{
	SelectedUnitMoveComponent = pathfindingUnitMoveComponent;
}

void APathfindingGrid::OverNewCell(FVector2D overedCellGridPos)
{
	if(overedCellGridPos != CurrentOveredCellGridPos)
	{
		CurrentOveredCellGridPos = overedCellGridPos;
		OnHighlightCurrentPath(false);
		if(bAUnitMoving == false && SelectedUnitMoveComponent != nullptr)
		{
			FindPathToTarget(SelectedUnitMoveComponent->GetCurrentGridIndex(), CurrentOveredCellGridPos);
			OnHighlightCurrentPath(true);
		}
	}
}

void APathfindingGrid::SelectNewCell(FVector2D selectedCellGridPos)
{
	const FS_Cell* selectedCellChecked = CellsMap.Find(CurrentSelectedCellGridPos);
	
	if(selectedCellChecked)
	{
		if(selectedCellChecked->TestHighlightFxInstanceRef != nullptr)
		{
			selectedCellChecked->TestHighlightFxInstanceRef->DestroyComponent();
		}
	}

	CurrentSelectedCellGridPos = selectedCellGridPos;

	if(bAUnitMoving == false)
	{
		OnGetGridPath.Broadcast(Path);
		selectedCellChecked = CellsMap.Find(CurrentSelectedCellGridPos);
		if(selectedCellChecked)
		{
			if(SelectedUnitMoveComponent != nullptr)
			{
				OnSelectNewCell.Broadcast(selectedCellChecked->CellWorldPos, CurrentSelectedCellGridPos);
				if(selectedCellChecked->TestHighlightFxInstanceRef != nullptr)
				{
					selectedCellChecked->TestHighlightFxInstanceRef->DestroyComponent();
				}
			}
		}
	}
}

FVector2D APathfindingGrid::FindGridCell(FVector worldPos, APathfindingGrid* grid)
{
	FVector dir = worldPos - grid->GetActorLocation();
	float x = FMath::Abs(UKismetMathLibrary::Round((dir.X - (grid->CellSize / 2.0f)) / grid->CellSize));
	float y = FMath::Abs(UKismetMathLibrary::Round((dir.Y -(grid->CellSize / 2.0f)) / grid->CellSize));

	FVector2D cellPos = FVector2D(x, y);
	return cellPos;
}


//Create Grid
void APathfindingGrid::CreateGrid(TArray<FS_Cell>& GridCells)
{
	ResetPath();
	
	AllCellsBP.Empty();

	//Clear all instances
	InstancedStaticMeshComponentCell->ClearInstances();
	InstancedStaticMeshComponentObstacle->ClearInstances();
	InstancedStaticMeshComponentCellReallyDifficult->ClearInstances();
	InstancedStaticMeshComponentCellDifficult->ClearInstances();

	//Get loop amount
	XAmount = int(GridSize.X / CellSize);
	YAmount = int(GridSize.Y / CellSize);

	float angleNorm = FMath::DegreesToRadians(MaxWalkableGroundAngle);

	//Array of actors for trace to ignore
	TArray<AActor*> IgnoreActors;
	//Add self to ignore actors
	IgnoreActors.Add(GetOwner());

	//Create hit result variable
	FHitResult HitArray;

	//Clear the cell array
	AllCells.Empty();

	//Get actor location and make it a local variable
	FVector actorLoc = GetActorLocation();

	CellRadius = float(CellSize / 2);

	int indx = 0;

	for (int x = 0; x < XAmount; x++)
	{
		for (int y = 0; y < YAmount; y++)
		{
			//Create local new cell struct
			FS_Cell newCell_Struct;

			FVector wPos = FVector(x * CellSize + actorLoc.X + CellRadius, y * CellSize + actorLoc.Y + CellRadius, actorLoc.Z);
			FRotator wRot = FRotator(0, 0, 0);

			if (bAlignToGround)
			{
				bool hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), GroundObjectType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

				if (hitGround)
				{
					wPos = HitArray.ImpactPoint;
					wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
					newCell_Struct.CellNormal = HitArray.ImpactNormal;
				}
				else
				{
					hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), ReallyDifficultGroundType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

					if (hitGround)
					{
						wPos = HitArray.ImpactPoint;
						wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
						newCell_Struct.CellNormal = HitArray.ImpactNormal;
					}
					else
					{
						hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), DifficultGroundType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

						if (hitGround)
						{
							wPos = HitArray.ImpactPoint;
							wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
							newCell_Struct.CellNormal = HitArray.ImpactNormal;
						}
						else
						{
							newCell_Struct.GroundType = EGroundType::E_None;
							if(PathfindingType == EPathfindingType::E_AStar)
							{
								newCell_Struct.CellCost = GetCellCost(EGroundType::E_None);
							}
						}
					}
				}
			}
			
			//Set new cell struct pos and world pos
			newCell_Struct.CellGridPos = FVector2D(x, y);
			newCell_Struct.CellWorldPos = wPos;

			float cellAngle = UKismetMathLibrary::Acos(FVector::DotProduct(HitArray.Normal, FVector(0, 0, 1)));

			if (cellAngle > angleNorm && bAlignToGround)
			{
				if(PathfindingType == EPathfindingType::E_FlowField)
				{
					newCell_Struct.CellCost = 255;

					newCell_Struct.GroundType = EGroundType::E_None;
				}

				if(PathfindingType == EPathfindingType::E_AStar)
				{
					newCell_Struct.CellCost = GetCellCost(EGroundType::E_None);
				}
			}
			else
			{
				if (bTraceForObstacles)
				{
					bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), wPos, wPos, CellRadius, ObstacleType, false, IgnoreActors,
						EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);
					if (hit)
					{
						newCell_Struct.GroundType = EGroundType::E_Impossible;
						if(PathfindingType == EPathfindingType::E_FlowField)
						{
							newCell_Struct.CellCost = 255;
						}
						else if(PathfindingType == EPathfindingType::E_AStar)
						{
							newCell_Struct.CellCost = GetCellCost(EGroundType::E_Impossible);
						}
					}
					else
					{
						hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), wPos, wPos, CellRadius, ReallyDifficultGroundType, false, IgnoreActors,
						EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);
						if (hit)
						{
							newCell_Struct.GroundType = EGroundType::E_ReallyDifficult;
							if(PathfindingType == EPathfindingType::E_FlowField)
							{
								newCell_Struct.CellCost = 255;
							}
							else if(PathfindingType == EPathfindingType::E_AStar)
							{
								newCell_Struct.CellCost = GetCellCost(EGroundType::E_ReallyDifficult);
							}
						}
						else
						{
							hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), wPos, wPos, CellRadius, DifficultGroundType, false, IgnoreActors,
						EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);
							if (hit)
							{
								newCell_Struct.GroundType = EGroundType::E_Difficult;
								if(PathfindingType == EPathfindingType::E_FlowField)
								{
									newCell_Struct.CellCost = 255;
								}
								else if(PathfindingType == EPathfindingType::E_AStar)
								{
									newCell_Struct.CellCost = GetCellCost(EGroundType::E_Difficult);
								}
							}
							else
							{
								newCell_Struct.GroundType = EGroundType::E_Normal;
								if(PathfindingType == EPathfindingType::E_FlowField)
								{
									newCell_Struct.CellCost = 1;
								}
								else if(PathfindingType == EPathfindingType::E_AStar)
								{
									newCell_Struct.CellCost = GetCellCost(EGroundType::E_Normal);
								}
							}
						}
					}
				}
				else
				{
					newCell_Struct.GroundType = EGroundType::E_Normal;
					if(PathfindingType == EPathfindingType::E_FlowField)
					{
						newCell_Struct.CellCost = 1;
					}
					else if(PathfindingType == EPathfindingType::E_AStar)
					{
						newCell_Struct.CellCost = GetCellCost(EGroundType::E_Normal);
					}
				}
			}

			


			newCell_Struct.CellBestCost = 65535;
			newCell_Struct.CellIndex = indx;

			GridCells.Add(newCell_Struct);
			AllCellsBP.Add(newCell_Struct);
			
			CellsMap.Add(FVector2D(x, y), newCell_Struct);


			//Add an instance for every cell
			if (bShowGridInGame == true)
			{
				FTransform trans = FTransform(wRot, wPos, FVector(CellSize / 100, CellSize / 100, 1));

				if (newCell_Struct.GroundType == E_Impossible)
				{
					InstancedStaticMeshComponentObstacle->AddInstance(trans, true);
				}
				else if (newCell_Struct.GroundType == E_ReallyDifficult)
				{
					InstancedStaticMeshComponentCellReallyDifficult->AddInstance(trans, true);
				}
				else if (newCell_Struct.GroundType == E_Difficult)
				{
					InstancedStaticMeshComponentCellDifficult->AddInstance(trans, true);
				}
				else
				{
					InstancedStaticMeshComponentCell->AddInstance(trans, true);
				}
			}



			//Add to index
			indx++;
		}
	}

}

void APathfindingGrid::DebugGrid()
{
	//Get loop amount
	XAmount = int(GridSize.X / CellSize);
	YAmount = int(GridSize.Y / CellSize);

	float angleNorm;

	if (bAlignToGround)
	{
		angleNorm = FMath::DegreesToRadians(MaxWalkableGroundAngle);
	}
	else
	{
		angleNorm = 2;
	}
	//Set the cell radius
	CellRadius = float(CellSize / 2);

	//Clear all instances
	InstancedStaticMeshComponentCell->ClearInstances();
	InstancedStaticMeshComponentObstacle->ClearInstances();
	InstancedStaticMeshComponentCellReallyDifficult->ClearInstances();
	InstancedStaticMeshComponentCellDifficult->ClearInstances();

	int indx = 0;

	//Create hit result variable
	FHitResult HitArray;

	//Array of actors for trace to ignore
	TArray<AActor*> IgnoreActors;
	//Add self to ignore actors
	IgnoreActors.Add(GetOwner());

	//Get Actor location
	FVector actorLoc = GetActorLocation();

	for (int x = 0; x < XAmount; x++)
	{
		for (int y = 0; y < YAmount; y++)
		{
			FVector wPos = FVector(x * CellSize + actorLoc.X + CellRadius, y * CellSize + actorLoc.Y + CellRadius, actorLoc.Z);
			FRotator wRot = FRotator(0, 0, 0);
			FVector newWPos = wPos;

			if (bAlignToGround)
			{
				bool hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), GroundObjectType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

				if (hitGround)
				{
					newWPos = HitArray.ImpactPoint;
					wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
				}
				else
				{
					hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), ReallyDifficultGroundType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

					if (hitGround)
					{
						newWPos = HitArray.ImpactPoint;
						wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
					}
					else
					{
						hitGround = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), wPos + FVector(0, 0, TraceHeight), FVector(wPos.X, wPos.Y, wPos.Z - TraceHeight * 2), DifficultGroundType, true, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 2);

						if (hitGround)
						{
							newWPos = HitArray.ImpactPoint;
							wRot = FRotationMatrix::MakeFromZ(HitArray.ImpactNormal).Rotator();
						}
					}
				}
			}

			FTransform trans = FTransform(wRot, newWPos, FVector(CellSize / 100, CellSize / 100, 1));

			bool tooSteep = false;
			bool hit = false;
			bool hitReallyDifficult = false;
			bool hitDifficult = false;

			if (UKismetMathLibrary::Acos(FVector::DotProduct(HitArray.Normal, FVector(0, 0, 1))) > angleNorm)
			{
				tooSteep = true;
			}

			if (bTraceForObstacles)
			{
				hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), newWPos, newWPos, CellRadius, ObstacleType, false, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 5);
				if(!hit)
				{
					hitReallyDifficult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), newWPos, newWPos, CellRadius, ReallyDifficultGroundType, false, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 5);

					if(!hitReallyDifficult)
					{
						hitDifficult = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), newWPos, newWPos, CellRadius, DifficultGroundType, false, IgnoreActors,
					EDrawDebugTrace::None, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 5);
					}
				}
			}
			else
			{
				hit = false;
				hitReallyDifficult = false;
				hitDifficult = false;
			}


			if (hit || tooSteep)
			{
				InstancedStaticMeshComponentObstacle->AddInstance(trans, true);
			}
			else if(hitReallyDifficult)
			{
				InstancedStaticMeshComponentCellReallyDifficult->AddInstance(trans, true);
			}
			else if(hitDifficult)
			{
				InstancedStaticMeshComponentCellDifficult->AddInstance(trans, true);
			}
			else
			{
				InstancedStaticMeshComponentCell->AddInstance(trans, true);
			}

			//Add to index
			indx++;
		}
	}
}


bool APathfindingGrid::IfNeigbourClose(const FVector2D currentPos, const FVector2D neighbourPos)
{
	if (currentPos[0] - 1 <= neighbourPos[0] && neighbourPos[0] <= currentPos[0] + 1 && currentPos[1] - 1 <= neighbourPos[1] && neighbourPos[1] <= currentPos[1] + 1) {
		return true;
	}
	else {
		return false;
	}
}

int APathfindingGrid::GetCellCost(TEnumAsByte<EGroundType> gridIndex)
{
	int cost = 0;
	
	switch (gridIndex)
	{
		case EGroundType::E_Normal:
			cost = 1;
			break; 
		case EGroundType::E_Difficult:
			cost = 2;
			break;
		case EGroundType::E_ReallyDifficult:
			cost = 5;
			break;
		case EGroundType::E_Impossible:
			cost = 999;
			break;
		case EGroundType::E_None:
			cost = 999;
			break;
			
		default: break; 
	}

	return cost;
}

TArray<FVector2D> APathfindingGrid::GetCellNeighbours(FVector2D gridIndex, bool displayDebug)
{
	TArray<FVector2D> outNeighbours;
	
	TArray<FVector2D> possibleNeighbours;
	possibleNeighbours.Add(FVector2D(1.0f, 0.0f));
	possibleNeighbours.Add(FVector2D(0.0f, 1.0f));
	possibleNeighbours.Add(FVector2D(-1.0f, 0.0f));
	possibleNeighbours.Add(FVector2D(0.0f, -1.0f));
	possibleNeighbours.Add(FVector2D(1.0f, 1.0f));
	possibleNeighbours.Add(FVector2D(-1.0f, -1.0f));
	possibleNeighbours.Add(FVector2D(1.0f, -1.0f));
	possibleNeighbours.Add(FVector2D(-1.0f, 1.0f));

	for (int i = 0; i < possibleNeighbours.Num(); ++i)
	{
		FVector2D cell = possibleNeighbours[i] + gridIndex;
		const FS_Cell* foundCell = CellsMap.Find(cell);
		if(foundCell && foundCell->GroundType != EGroundType::E_None && foundCell->GroundType != EGroundType::E_Impossible
			 && foundCell->PathfindingUnitComponentRef == nullptr)
		{
			if(displayDebug)
			{
				UE_LOG(LogTemp, Warning, TEXT("Neighbour Found X : %f Y : %f"), cell.X, cell.Y);
			}
			outNeighbours.AddUnique(cell);
		}
	}

	return outNeighbours;
}

FVector2D APathfindingGrid::GetClosestCellNeighbour(FVector2D gridIndexTarget, FVector2D gridIndexSource, bool& successCellFound, bool displayDebug)
{
	successCellFound = false;
	
	const FS_Cell* cellSource = CellsMap.Find(gridIndexSource);
	
	TArray<FVector2D> cellNeighboursPos = GetCellNeighbours(gridIndexTarget, displayDebug);
	if(displayDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Search Neighbours of Cell : X : %f Y : %f"), gridIndexTarget.X, gridIndexTarget.Y);
		UE_LOG(LogTemp, Warning, TEXT("Neighbours Found Count: %d"), cellNeighboursPos.Num());
	}
	
	float closestDistance = FLT_MAX;
	FVector2D closestCell = gridIndexTarget;
	
	if(cellNeighboursPos.IsEmpty() == false)
	{
		for (int i = 0; i < cellNeighboursPos.Num(); ++i)
		{
			const FS_Cell* cellNeighbour = CellsMap.Find(cellNeighboursPos[i]);
			float distance = FVector::Dist(cellSource->CellWorldPos, cellNeighbour->CellWorldPos);
			if(distance <= closestDistance)
			{
				closestCell = cellNeighboursPos[i];
				closestDistance = distance;
				successCellFound = true;
			}
		}
	}

	return closestCell;
}

int APathfindingGrid::GetEstimatedCostToTarget(FVector2D currentCell, FVector2D targetCell)
{
	FVector2D distCells = currentCell - targetCell;
	int estimatedCostValue = FMath::RoundToInt(FMath::Abs(distCells.X) + FMath::Abs(distCells.Y));
	return estimatedCostValue;
}

void APathfindingGrid::ResetPath()
{
	for (int i = 0; i < Path.Num(); ++i)
	{
		const FS_Cell* previousPathCell = CellsMap.Find(Path[i]);
		if(previousPathCell && previousPathCell->TestHighlightFxInstanceRef != nullptr)
		{
			previousPathCell->TestHighlightFxInstanceRef->DestroyComponent();
		}
	}
	
	Path.Empty();
}

void APathfindingGrid::FindPathToTarget(FVector2D startCell, FVector2D targetCell)
{
	FVector2D startCellL = startCell;
	FVector2D targetCellL = targetCell;

	const FS_Cell* resetStartCell = CellsMap.Find(startCellL);
	if(resetStartCell && resetStartCell->TestHighlightFxInstanceRef != nullptr)
	{
		resetStartCell->TestHighlightFxInstanceRef->DestroyComponent();
	}

	const FS_Cell* resetTargetCell = CellsMap.Find(targetCellL);
	if(resetTargetCell && resetTargetCell->TestHighlightFxInstanceRef != nullptr)
	{
		resetTargetCell->TestHighlightFxInstanceRef->DestroyComponent();
	}
	
	ResetPath();
	
	TArray<FVector2D> openSet;
	TArray<FVector2D> closedSet;
	
	FVector2D currentCell;
	FVector2D openSetCheapest;
	FVector2D currentNeighbour;
	
	int currentNeighbourCostFromStart = 0;

	const FS_Cell* foundCell = CellsMap.Find(targetCellL);
	if(foundCell && foundCell->GroundType != EGroundType::E_None && foundCell->GroundType != EGroundType::E_Impossible
		&& foundCell->PathfindingUnitComponentRef == nullptr)
	{
		TArray<FVector2D> mapKeys;
		CellsMap.GetKeys(mapKeys);
		for (int i = 0; i < mapKeys.Num(); ++i)
		{
			const FS_Cell* mapCell = CellsMap.Find(mapKeys[i]);
			FS_Cell newCellStruct;

			newCellStruct.PathfindingUnitComponentRef = mapCell->PathfindingUnitComponentRef;
			newCellStruct.GroundType = mapCell->GroundType;
			newCellStruct.CellWorldPos = mapCell->CellWorldPos;
			newCellStruct.CellGridPos = mapCell->CellGridPos;
			newCellStruct.CellCost = mapCell->CellCost;
			newCellStruct.CellFinalCost = 999;
			newCellStruct.CellCostFromStart = 999;
			newCellStruct.CellEstimatedCostToTarget = 999;
			newCellStruct.PreviousCellGridPos = FVector2D(0.0f, 0.0f);
			
			CellsMap.Add(mapKeys[i], newCellStruct);
		}
		
		const FS_Cell* startCellChecked = CellsMap.Find(startCellL);
		FS_Cell newCellStruct;

		int estimatedCostToTarget = GetEstimatedCostToTarget(startCellL, targetCellL);

		newCellStruct.PathfindingUnitComponentRef = startCellChecked->PathfindingUnitComponentRef;
		newCellStruct.GroundType = startCellChecked->GroundType;
		newCellStruct.CellWorldPos = startCellChecked->CellWorldPos;
		newCellStruct.CellGridPos = startCellChecked->CellGridPos;
		newCellStruct.CellCost = startCellChecked->CellCost;
		newCellStruct.CellFinalCost = estimatedCostToTarget;
		newCellStruct.CellCostFromStart = 0;
		newCellStruct.CellEstimatedCostToTarget = estimatedCostToTarget;
		newCellStruct.PreviousCellGridPos = FVector2D(0.0f, 0.0f);

		CellsMap.Add(startCellL, newCellStruct);


		currentCell = startCellL;
		openSet.AddUnique(currentCell);
		while(openSet.Num() > 0)
		{
			openSetCheapest = openSet[0];
			for (int i = 0; i < openSet.Num(); ++i)
			{
				const FS_Cell* openSetCellChecked = CellsMap.Find(openSet[i]);
				const FS_Cell* openSetCheapestCell = CellsMap.Find(openSetCheapest);

				if(openSetCellChecked->CellFinalCost < openSetCheapestCell->CellFinalCost)
				{
					openSetCheapest = openSet[i];
				}
				else
				{
					if(openSetCellChecked->CellFinalCost == openSetCheapestCell->CellFinalCost)
					{
						if(openSetCellChecked->CellEstimatedCostToTarget < openSetCheapestCell->CellEstimatedCostToTarget)
						{
							openSetCheapest = openSet[i];
						}
					}
				}
			}

			currentCell = openSetCheapest;
			openSet.Remove(currentCell);
			closedSet.AddUnique(currentCell);

			TArray<FVector2D> neighbours = GetCellNeighbours(currentCell);
			for (int i = 0; i < neighbours.Num(); ++i)
			{
				currentNeighbour = neighbours[i];
				if(closedSet.Contains(currentNeighbour) == false)
				{
					const FS_Cell* currentCellChecked = CellsMap.Find(currentCell);
					const FS_Cell* currentNeighbourCellChecked = CellsMap.Find(currentNeighbour);
					currentNeighbourCostFromStart = currentCellChecked->CellCostFromStart + currentNeighbourCellChecked->CellCost;

					if(openSet.Contains(currentNeighbour) == false)
					{
						openSet.AddUnique(currentNeighbour);

						currentNeighbourCellChecked = CellsMap.Find(currentNeighbour);
						if(currentNeighbourCostFromStart < currentNeighbourCellChecked->CellCostFromStart)
						{
							FS_Cell newNeighbourCellStruct;
							newNeighbourCellStruct.PathfindingUnitComponentRef = currentNeighbourCellChecked->PathfindingUnitComponentRef;
							newNeighbourCellStruct.GroundType = currentNeighbourCellChecked->GroundType;
							newNeighbourCellStruct.CellWorldPos = currentNeighbourCellChecked->CellWorldPos;
							newNeighbourCellStruct.CellGridPos = currentNeighbourCellChecked->CellGridPos;
							newNeighbourCellStruct.CellCost = currentNeighbourCellChecked->CellCost;

							int newNeighbourCellEstimatedCostToTarget = GetEstimatedCostToTarget(currentNeighbour, targetCellL);
							newNeighbourCellStruct.CellFinalCost = currentNeighbourCostFromStart + newNeighbourCellEstimatedCostToTarget;

							newNeighbourCellStruct.CellCostFromStart = currentNeighbourCostFromStart;
							newNeighbourCellStruct.CellEstimatedCostToTarget = newNeighbourCellEstimatedCostToTarget;
							newNeighbourCellStruct.PreviousCellGridPos = currentCell;
							
							CellsMap.Add(currentNeighbour, newNeighbourCellStruct);

							if(currentNeighbour == targetCellL)
							{
								Path = RetracePath(targetCellL, startCellL);
								return;
							}
						}
					}
					else
					{
						currentNeighbourCellChecked = CellsMap.Find(currentNeighbour);
						if(currentNeighbourCostFromStart < currentNeighbourCellChecked->CellCostFromStart)
						{
							FS_Cell newNeighbourCellStruct;
							newNeighbourCellStruct.PathfindingUnitComponentRef = currentNeighbourCellChecked->PathfindingUnitComponentRef;
							newNeighbourCellStruct.GroundType = currentNeighbourCellChecked->GroundType;
							newNeighbourCellStruct.CellWorldPos = currentNeighbourCellChecked->CellWorldPos;
							newNeighbourCellStruct.CellGridPos = currentNeighbourCellChecked->CellGridPos;
							newNeighbourCellStruct.CellCost = currentNeighbourCellChecked->CellCost;

							int newNeighbourCellEstimatedCostToTarget = GetEstimatedCostToTarget(currentNeighbour, targetCellL);
							newNeighbourCellStruct.CellFinalCost = currentNeighbourCostFromStart + newNeighbourCellEstimatedCostToTarget;

							newNeighbourCellStruct.CellCostFromStart = currentNeighbourCostFromStart;
							newNeighbourCellStruct.CellEstimatedCostToTarget = newNeighbourCellEstimatedCostToTarget;
							newNeighbourCellStruct.PreviousCellGridPos = currentCell;
							
							CellsMap.Add(currentNeighbour, newNeighbourCellStruct);

							if(currentNeighbour == targetCellL)
							{
								Path = RetracePath(targetCellL, startCellL);
								return;
							}
						}
					}
				}
			}
		}

		return;
	}
}

TArray<FVector2D> APathfindingGrid::RetracePath(FVector2D targetCell, FVector2D startCell)
{
	FVector2D currentCell = targetCell;
	
	TArray<FVector2D> invertedPath;
	TArray<FVector2D> outPath;

	while(currentCell != startCell)
	{
		invertedPath.AddUnique(currentCell);
		const FS_Cell* currentCellChecked = CellsMap.Find(currentCell);
		currentCell = currentCellChecked->PreviousCellGridPos;
	}

	for (int i = invertedPath.Num() - 1; i >= 0; --i)
	{
		outPath.AddUnique(invertedPath[i]);
	}

	return outPath;
}


void APathfindingGrid::GenerateFlowField(const TArray<FS_Cell> GridCells, const FVector goal, TMap<FVector2D, FVector>& directionMap, FVector& goalWorldPos)
{
	TArray<PathfindingCellClass> _gridCells;

	for (int i = 0; i < GridCells.Num(); i++)
	{
		FS_Cell oldCell = GridCells[i];
		PathfindingCellClass newCell;
		newCell.cost = oldCell.CellCost;
		newCell.bestCost = oldCell.CellBestCost;
		newCell.pos = oldCell.CellGridPos;
		newCell.worldPos = oldCell.CellWorldPos;
		newCell.index = oldCell.CellIndex;
		newCell.dir = oldCell.CellDir;
		newCell.normal = oldCell.CellNormal;

		_gridCells.Add(newCell);
	}

	AllCells = _gridCells;

	//Finds the closest 2D vector to the input world position
	FVector relativePos = FVector(GetActorLocation() - goal + FVector(CellRadius, CellRadius, 0));
	FVector2D goal2DPos = FVector2D(UKismetMathLibrary::Abs(UKismetMathLibrary::Round(relativePos.X / CellSize)), UKismetMathLibrary::Abs(UKismetMathLibrary::Round(relativePos.Y / CellSize)));

	//Set the output goal world position
	goalWorldPos = FVector(GetActorLocation().X + goal2DPos.X * CellSize + CellRadius, GetActorLocation().Y + goal2DPos.Y * CellSize + CellRadius, goal.Z);

	//Set the goal position to the function input
	GoalPosition = goal2DPos;

	//Creates the flow field
	TArray<PathfindingCellClass> newGridCells;
	newGridCells = CreateFlowField(CreateIntegrationField(goal2DPos, _gridCells));

	TMap<FVector2D, FVector> _dirMap;

	//DirMap.Empty();

	for (int i = 0; i < newGridCells.Num(); i++)
	{
		_dirMap.Add(_gridCells[i].pos, newGridCells[i].dir);
	}
	directionMap = _dirMap;
}

//Create Integration Field
TArray<PathfindingCellClass> APathfindingGrid::CreateIntegrationField(const FVector2D destinationPos, const TArray<PathfindingCellClass> gridCells)
{
	TArray<PathfindingCellClass> _gridCells = gridCells;
	int destIndex = 0;

	for (int i = 0; i < _gridCells.Num(); i++)
	{
		if (destinationPos == _gridCells[i].pos)
		{
			destIndex = _gridCells[i].index;
			_gridCells[i].bestCost = 0;
			_gridCells[i].cost = 0;
			break;
		}
	}

	PathfindingCellClass destCell = _gridCells[destIndex];

	TArray<PathfindingCellClass> gridCellsToCheck;

	int ind = 0;

	gridCellsToCheck.Add(destCell);

	while (gridCellsToCheck.Num() > 0) {

		PathfindingCellClass curCell = gridCellsToCheck[0];

		int nIndex = 0;
		int currentIndex = curCell.index;

		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
			{
				nIndex = currentIndex + YAmount;
			}
			if (i == 1)
			{
				nIndex = currentIndex + 1;
			}
			if (i == 2)
			{
				nIndex = currentIndex - YAmount;
			}
			if (i == 3)
			{
				nIndex = currentIndex - 1;
			}

			if (nIndex < 0 || nIndex > _gridCells.Num() - 1) { continue; }

			PathfindingCellClass neighbourCell = _gridCells[nIndex];

			if (neighbourCell.cost == 255) { continue; }
			if (FMath::Abs(curCell.worldPos.Z - neighbourCell.worldPos.Z) > CellSize) { continue; }

			//UE_LOG(LogTemp, Warning, TEXT("Lenght: %f"), FVector::DotProduct(FVector(curCell.normal.X,curCell.normal.Y,0), FVector(neighbourCell.normal.X, neighbourCell.normal.Y, 0)));

			if (IfNeigbourClose(curCell.pos, neighbourCell.pos) == false) { continue; }
			if (neighbourCell.cost + curCell.bestCost < neighbourCell.bestCost)
			{
				neighbourCell.bestCost = int(neighbourCell.cost + curCell.bestCost);
				_gridCells[neighbourCell.index].bestCost = int(neighbourCell.cost + curCell.bestCost);
				gridCellsToCheck.Add(_gridCells[neighbourCell.index]);
				ind++;

			}
		}
		gridCellsToCheck.RemoveAt(0);
	}
	return _gridCells;
}

TArray<PathfindingCellClass> APathfindingGrid::CreateFlowField(const TArray<PathfindingCellClass> gridCells)
{
	TArray<PathfindingCellClass> _gridCells = gridCells;

	InstancedStaticMeshComponentArrow->ClearInstances();

	for (int j = 0; j < _gridCells.Num(); j++)
	{

		PathfindingCellClass curCell = _gridCells[j];

		int _bestCost = curCell.bestCost;

		float bestDot = 0;

		int currentIndex = j;

		int nIndex = 0;

		for (int i = 0; i < 8; i++)
		{
			if (i == 0)
			{
				nIndex = currentIndex + YAmount;
			}
			if (i == 1)
			{
				nIndex = currentIndex + 1;
			}
			if (i == 2)
			{
				nIndex = currentIndex - YAmount;
			}
			if (i == 3)
			{
				nIndex = currentIndex - 1;
			}
			if (i == 4)
			{
				nIndex = currentIndex + YAmount + 1;
			}
			if (i == 5)
			{
				nIndex = currentIndex - YAmount + 1;
			}
			if (i == 6)
			{
				nIndex = currentIndex - YAmount - 1;
			}
			if (i == 7)
			{
				nIndex = currentIndex + YAmount - 1;
			}

			if (nIndex < 0 || nIndex > _gridCells.Num() - 1) { continue; }

			PathfindingCellClass neighbourCell = _gridCells[nIndex];


			//UE_LOG(LogTemp, Warning, TEXT("Lenght: %f"), UKismetMathLibrary::Abs((_gridCells[nIndex].worldPos.Z - curCell.worldPos.Z)));
			if (IfNeigbourClose(curCell.pos, neighbourCell.pos) == false) { continue; }
			if (IsDiagonalValid(i, currentIndex, nIndex) == false) { continue; }
			if (FMath::Abs(curCell.worldPos.Z - neighbourCell.worldPos.Z) > CellSize) { continue; }

			float vDot = FVector::DotProduct(curCell.normal, neighbourCell.normal);

			//if (vDot < bestDot) { continue; }

			if (neighbourCell.bestCost < _bestCost)
			{
				_bestCost = neighbourCell.bestCost;
				_gridCells[currentIndex].dir = UKismetMathLibrary::GetDirectionUnitVector(curCell.worldPos, neighbourCell.worldPos);
				//bestDot = vDot;
			}

		}
		if (bShowArrowsInGame && _gridCells[currentIndex].dir.IsZero() == false)
		{
			FRotator newRot = FRotationMatrix::MakeFromX(_gridCells[currentIndex].dir).Rotator();
			InstancedStaticMeshComponentArrow->AddInstance(FTransform(newRot, FVector(curCell.worldPos) + FVector(0, 0, 1), FVector(CellSize / 100)), true);
		}
	}
	return _gridCells;

}

bool APathfindingGrid::IsDiagonalValid(const int loopIndex, const int curIndex, const int neighbourIndex)
{
	int maxIndex = AllCells.Num();

	if (loopIndex == 4 || loopIndex == 5)
	{
		if (curIndex + 1 < maxIndex && neighbourIndex - 1 > -1)
		{
			if (AllCells[curIndex + 1].cost != 255 && AllCells[neighbourIndex - 1].cost != 255)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}
	}
	else if (loopIndex == 6 || loopIndex == 7)
	{
		if (curIndex - 1 > -1 && neighbourIndex + 1 < maxIndex)
		{
			if (AllCells[curIndex - 1].cost != 255 && AllCells[neighbourIndex + 1].cost != 255)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}

}