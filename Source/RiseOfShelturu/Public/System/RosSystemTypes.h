#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "RosSystemTypes.generated.h"

class AController;
class AGameModeBase;

/**
 * ERosGamePhase
 *
 *	Defines current phase of game.
 */
UENUM(BlueprintType)
enum class ERosGamePhase : uint8
{
 E_ExplorationPhase       UMETA(DisplayName="Exploration"),
 E_BattlePhase        UMETA(DisplayName="Battle"),
 E_NonePhase        UMETA(DisplayName="None"),
 E_DebugPhase      UMETA(DisplayName="DebugPhase"),
};

/**
 * Post login event, triggered when a player or bot joins the game as well as after seamless and non seamless travel
 *
 * This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRosGameModePlayerInitialized, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

/**
 * Set Game Phase, call on player game phase changed
 *
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetGamePhaseSignature, ERosGamePhase, GamePhase);
