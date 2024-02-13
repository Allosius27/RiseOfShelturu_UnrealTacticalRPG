#pragma once
#include "UObject/ObjectMacros.h"

#include "RosDevelopmentTypes.generated.h"


UENUM()
enum class ECheatExecutionTime
{
	// When the cheat manager is created
	OnCheatManagerCreated,

	// When a pawn is possessed by a player
	OnPlayerPawnPossession
};

USTRUCT()
struct FRosCheatToRun
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ECheatExecutionTime Phase = ECheatExecutionTime::OnPlayerPawnPossession;

	UPROPERTY(EditAnywhere)
	FString Cheat;
};

