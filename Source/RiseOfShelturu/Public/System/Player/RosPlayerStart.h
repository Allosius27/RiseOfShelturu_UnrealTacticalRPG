// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "RosPlayerStart.generated.h"

class AController;

/**
 * ALyraPlayerStart
 * 
 * Base player starts that can be used by a lot of modes.
 */
UCLASS(Abstract, Config = Game)
class RISEOFSHELTURU_API ARosPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	ARosPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

protected:
	
	/** The controller that claimed this PlayerStart */
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	/** Interval in which we'll check if this player start is not colliding with anyone anymore */
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart|Claiming")
	float ExpirationCheckInterval = 1.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStart|Components")
	class USceneComponent* PlayerUnitsSpawnPointsParent;

#pragma endregion

#pragma region EVENTS

	/** Handle to track expiration recurring timer */
	FTimerHandle ExpirationTimerHandle;

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion

	/** Did this player start get claimed by a controller already? */
	bool IsClaimed() const;

	/** If this PlayerStart was not claimed, claim it for ClaimingController */
	bool TryClaim(AController* OccupyingController);

	UFUNCTION(BlueprintCallable, Category = "PlayerStart")
	TArray<AActor*> SpawnPlayerUnits(TArray<TSubclassOf<AActor>> unitsActorsClasses);
	
protected:

	/** Check if this PlayerStart is still claimed */
	void CheckUnclaimed();

#pragma endregion
};
