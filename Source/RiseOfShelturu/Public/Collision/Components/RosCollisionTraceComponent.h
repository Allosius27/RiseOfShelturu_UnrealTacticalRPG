// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RosCollisionTraceComponent.generated.h"


class URosGameManager;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISEOFSHELTURU_API URosCollisionTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	URosCollisionTraceComponent();

#pragma region PROPERTIES

public:

protected:

private:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CollisionTrace|GameInstance")
	URosGameManager* GetRosGameManager() const;

	UFUNCTION(BlueprintCallable, Category = "CollisionTrace|Tracing")
	bool LineTraceByChannel(FVector start, FVector end, ECollisionChannel TraceChannel, EDrawDebugTrace::Type drawDebugType, FHitResult& OutHits);
	
	UFUNCTION(BlueprintCallable, Category = "CollisionTrace|Tracing")
	bool SphereTraceMultiByChannel(FVector start, FVector end, float radius, ECollisionChannel TraceChannel, TArray<FHitResult>& OutHits);

	UFUNCTION(BlueprintCallable, Category = "CollisionTrace|Tracing")
	bool SphereTraceMulti(FVector start, FVector end, float radius, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, TArray<FHitResult>& OutHits);
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion
		
};
