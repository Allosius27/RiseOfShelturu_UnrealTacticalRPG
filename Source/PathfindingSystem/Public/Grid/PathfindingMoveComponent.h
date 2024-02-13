// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingMoveComponent.generated.h"

class APathfindingGrid;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHFINDINGSYSTEM_API UPathfindingMoveComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// Sets default values for this component's properties
	UPathfindingMoveComponent();

#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	float GoalStepAcceptanceDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	float GoalFinalAcceptanceDistance = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	float RotationSpeed = 5.0f;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent|Components")
	USceneComponent* OwnerPositionComponent;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	APathfindingGrid* Grid;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentGridIndex, Category = "PathfindingMoveComponent")
	FVector2D CurrentGridIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	TArray<FVector2D> CurrentPath;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	int CurrentPathIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	FVector GoalPos;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetIsMoving, Category = "PathfindingMoveComponent")
	bool IsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathfindingMoveComponent")
	FVector MoveDirection;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma region GETTERS

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PathfindingMoveComponent")
		FVector2D GetCurrentGridIndex() const {return  CurrentGridIndex;}

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PathfindingMoveComponent")
		bool GetIsMoving() const {return IsMoving;}

	#pragma endregion 
	
	UFUNCTION(BlueprintCallable)
	void InitComponent(USceneComponent* posComponent);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void SetClosestGridCell(bool snapUnitOwner = false);
	
	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	FVector2D GetClosestGridCell();

	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void ResetUnitOnCurrentCell();
	
	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void SetUnitOnNewCell(FVector2D cellGridIndex, bool snapUnitOwner = false);

	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void SetPathfindingPath(TArray<FVector2D> newPath);

	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void MoveAlongPathfindingPath();

	UFUNCTION(BlueprintCallable, Category = "PathfindingMoveComponent")
	void StopPathfindingMove();
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Start();

private:


#pragma endregion
};
