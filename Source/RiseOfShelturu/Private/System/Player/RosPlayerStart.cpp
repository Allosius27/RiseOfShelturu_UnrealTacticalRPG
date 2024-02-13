// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/RosPlayerStart.h"

ARosPlayerStart::ARosPlayerStart(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PlayerUnitsSpawnPointsParent = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerUnitsSpawnPointsParent"));
	PlayerUnitsSpawnPointsParent->SetupAttachment(RootComponent);
}

bool ARosPlayerStart::IsClaimed() const
{
	return ClaimingController != nullptr;
}

bool ARosPlayerStart::TryClaim(AController* OccupyingController)
{
	if (OccupyingController != nullptr && !IsClaimed())
	{
		ClaimingController = OccupyingController;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ExpirationTimerHandle, FTimerDelegate::CreateUObject(this, &ARosPlayerStart::CheckUnclaimed), ExpirationCheckInterval, true);
		}
		return true;
	}
	return false;
}

TArray<AActor*> ARosPlayerStart::SpawnPlayerUnits(TArray<TSubclassOf<AActor>> unitsActorsClasses)
{
	TArray<AActor*> unitsActors;
	
	for (int i = 0; i < unitsActorsClasses.Num(); ++i)
	{
		FTransform transform = GetTransform();
		
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParams.bNoFail = true;
		spawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
		
		if(PlayerUnitsSpawnPointsParent->GetNumChildrenComponents() > i)
		{
			transform = PlayerUnitsSpawnPointsParent->GetChildComponent(i)->GetComponentTransform();
		}

		AActor* unitActor = GetWorld()->SpawnActor<AActor>(unitsActorsClasses[i], transform, spawnParams);
		unitsActors.Add(unitActor);
	}

	return unitsActors;
}

void ARosPlayerStart::CheckUnclaimed()
{
	if (ClaimingController != nullptr && ClaimingController->GetPawn() != nullptr)
	{
		ClaimingController = nullptr;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ExpirationTimerHandle);
		}
	}
}
