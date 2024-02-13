// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/PathfindingMoveComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Grid/PathfindingGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPathfindingMoveComponent::UPathfindingMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathfindingMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Start();
}

void UPathfindingMoveComponent::Start()
{
	TSubclassOf<APathfindingGrid> pathfindingGridClass = APathfindingGrid::StaticClass();
	Grid = Cast<APathfindingGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), pathfindingGridClass));
}


void UPathfindingMoveComponent::InitComponent(USceneComponent* posComponent)
{
	OwnerPositionComponent = posComponent;
}

// Called every frame
void UPathfindingMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveAlongPathfindingPath();
}

void UPathfindingMoveComponent::SetClosestGridCell(bool snapUnitOwner)
{
	FVector2D closestIndex = GetClosestGridCell();

	SetUnitOnNewCell(closestIndex, snapUnitOwner);
}

FVector2D UPathfindingMoveComponent::GetClosestGridCell()
{
	float closestDistance = 99999999999.0f;
	FVector2D closestIndex = FVector2D(0.0f, 0.0f);

	TArray<FVector2D> mapKeys;
	Grid->CellsMap.GetKeys(mapKeys);
	for (int i = 0; i < mapKeys.Num(); ++i)
	{
		const FS_Cell* mapCell = Grid->CellsMap.Find(mapKeys[i]);
		float distance = (mapCell->CellWorldPos - GetOwner()->GetActorLocation()).Size();
		if(distance < closestDistance)
		{
			closestDistance = distance;
			closestIndex = mapKeys[i];
		}
	}

	return closestIndex;
}

void UPathfindingMoveComponent::ResetUnitOnCurrentCell()
{
	for (int i = 0; i < CurrentPath.Num(); ++i)
	{
		const FS_Cell* mapCell = Grid->CellsMap.Find(CurrentPath[i]);
		if(mapCell)
		{
			if(mapCell->TestHighlightFxInstanceRef != nullptr)
			{
				mapCell->TestHighlightFxInstanceRef->DestroyComponent();
			}
		}
	}
	
	const FS_Cell* mapCell = Grid->CellsMap.Find(CurrentGridIndex);
	FS_Cell newCellStruct;

	newCellStruct.GroundType = mapCell->GroundType;
	newCellStruct.CellWorldPos = mapCell->CellWorldPos;
	newCellStruct.CellGridPos = mapCell->CellGridPos;
	newCellStruct.CellCost = mapCell->CellCost;
	newCellStruct.CellFinalCost = mapCell->CellFinalCost;
	newCellStruct.CellCostFromStart = mapCell->CellCostFromStart;
	newCellStruct.CellEstimatedCostToTarget = mapCell->CellEstimatedCostToTarget;
	newCellStruct.PreviousCellGridPos = mapCell->PreviousCellGridPos;
			
	Grid->CellsMap.Add(CurrentGridIndex, newCellStruct);
}

void UPathfindingMoveComponent::SetUnitOnNewCell(FVector2D cellGridIndex, bool snapUnitOwner)
{
	const FS_Cell* mapCell = Grid->CellsMap.Find(CurrentGridIndex);
	FS_Cell newCellStruct;
	
	newCellStruct.GroundType = mapCell->GroundType;
	newCellStruct.CellWorldPos = mapCell->CellWorldPos;
	newCellStruct.CellGridPos = mapCell->CellGridPos;
	newCellStruct.CellCost = mapCell->CellCost;
	newCellStruct.CellFinalCost = mapCell->CellFinalCost;
	newCellStruct.CellCostFromStart = mapCell->CellCostFromStart;
	newCellStruct.CellEstimatedCostToTarget = mapCell->CellEstimatedCostToTarget;
	newCellStruct.PreviousCellGridPos = mapCell->PreviousCellGridPos;
			
	Grid->CellsMap.Add(CurrentGridIndex, newCellStruct);

	CurrentGridIndex = cellGridIndex;

	const FS_Cell* newMapCell = Grid->CellsMap.Find(CurrentGridIndex);
	
	newCellStruct.PathfindingUnitComponentRef = this;
	newCellStruct.GroundType = newMapCell->GroundType;
	newCellStruct.CellWorldPos = newMapCell->CellWorldPos;
	newCellStruct.CellGridPos = newMapCell->CellGridPos;
	newCellStruct.CellCost = newMapCell->CellCost;
	newCellStruct.CellFinalCost = newMapCell->CellFinalCost;
	newCellStruct.CellCostFromStart = newMapCell->CellCostFromStart;
	newCellStruct.CellEstimatedCostToTarget = newMapCell->CellEstimatedCostToTarget;
	newCellStruct.PreviousCellGridPos = newMapCell->PreviousCellGridPos;

	Grid->CellsMap.Add(CurrentGridIndex, newCellStruct);

	if(snapUnitOwner)
	{
		ACharacter* characterOwner = Cast<ACharacter>(GetOwner());
		if(characterOwner)
		{
			GetOwner()->SetActorLocation(FVector(newMapCell->CellWorldPos.X, newMapCell->CellWorldPos.Y, newMapCell->CellWorldPos.Z + characterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
		}
		else
		{
			GetOwner()->SetActorLocation(newMapCell->CellWorldPos);
		}
	}
}

void UPathfindingMoveComponent::SetPathfindingPath(TArray<FVector2D> newPath)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set Pathfinding Path"));
	
	CurrentPath = newPath;

	CurrentPath.Insert(CurrentGridIndex, 0);

	SetUnitOnNewCell(CurrentPath.Last());

	Grid->bAUnitMoving = true;

	for (int i = 0; i < CurrentPath.Num(); ++i)
	{
		const FS_Cell* mapCell = Grid->CellsMap.Find(CurrentPath[i]);
		if(mapCell)
		{
			if(mapCell->TestHighlightFxInstanceRef != nullptr)
			{
				mapCell->TestHighlightFxInstanceRef->DestroyComponent();
			}
		}
	}

	const FS_Cell* currentCell = Grid->CellsMap.Find(CurrentPath[CurrentPathIndex]);
	GoalPos = currentCell->CellWorldPos;
	
	IsMoving = true;
}

void UPathfindingMoveComponent::MoveAlongPathfindingPath()
{
	if(Grid == nullptr)
	{
		return;
	}

	if(IsMoving)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Pathfinding Moving"));
		
		float goalDistance = FVector::Distance(OwnerPositionComponent->GetComponentLocation(), GoalPos);
		
		float acceptanceDistance = GoalStepAcceptanceDistance;
		if(CurrentPathIndex >= CurrentPath.Num() - 1)
		{
			acceptanceDistance = GoalFinalAcceptanceDistance;
		}
		
		if(goalDistance > acceptanceDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal Distance : %f"), goalDistance);
			MoveDirection = UKismetMathLibrary::GetDirectionUnitVector(GetOwner()->GetActorLocation(), GoalPos);
			APawn* ownerPawn = Cast<APawn>(GetOwner());
			if(ownerPawn != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Unit Pathfinding Add Movement Input"));
				// // Définissez la vitesse de déplacement
				// float MovementSpeed = 500.0f; // Exemple : vitesse de déplacement
				// // Calculez le déplacement en fonction de la direction et de la vitesse
				// FVector Movement = MoveDirection * MovementSpeed * ownerPawn->GetWorld()->GetDeltaSeconds();
				// // Mettez à jour la position du personnage
				// ownerPawn->SetActorLocation(ownerPawn->GetActorLocation() + Movement);

				ownerPawn->AddMovementInput(MoveDirection, 1.0f, true);
			}

			if(goalDistance > GoalStepAcceptanceDistance)
			{
				FRotator rotationLooked = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GoalPos);
				FRotator rotationInterp = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), rotationLooked, GetWorld()->GetDeltaSeconds(), RotationSpeed);
				GetOwner()->SetActorRotation(FRotator(0.0f, rotationInterp.Yaw, 0.0f));
			}
		}
		else
		{
			CurrentPathIndex += 1;
			if(CurrentPathIndex < CurrentPath.Num())
			{
				const FS_Cell* currentCell = Grid->CellsMap.Find(CurrentPath[CurrentPathIndex]);
				GoalPos = currentCell->CellWorldPos;
			}
			else
			{
				StopPathfindingMove();
			}
		}
	}
}

void UPathfindingMoveComponent::StopPathfindingMove()
{
	FRotator rotationLooked = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GoalPos);
	FRotator rotationInterp = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), rotationLooked, GetWorld()->GetDeltaSeconds(), RotationSpeed);
	GetOwner()->SetActorRotation(FRotator(0.0f, rotationInterp.Yaw, 0.0f));
	
	CurrentPathIndex = 0;
	Grid->bAUnitMoving = false;
	IsMoving = false;
}

