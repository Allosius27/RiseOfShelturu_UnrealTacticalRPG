// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameStates/RosGameState.h"

#include "AIController.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Grid/PathfindingGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "System/Player/RosPlayerController.h"
#include "System/Player/RosTacticPawnController.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Actors/Heroes/RosHeroUnit.h"
#include "Units/AI/Components/RosAIStateMachineBrain.h"
#include "Units/Components/Common/RosStatsUnitComponent.h"

class APlayerState;
class FLifetimeProperty;

extern ENGINE_API float GAverageFPS;

ARosGameState::ARosGameState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URosAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	ServerFPS = 0.0f;

	CurrentGamePhase = ERosGamePhase::E_NonePhase;
}

UAbilitySystemComponent* ARosGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARosGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARosGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void ARosGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARosGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ARosGameState::RemovePlayerState(APlayerState* PlayerState)
{
	//@TODO: This isn't getting called right now (only the 'rich' AGameMode uses it, not AGameModeBase)
	// Need to at least comment the engine code, and possibly move things around
	Super::RemovePlayerState(PlayerState);
}

void ARosGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}

void ARosGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
}

void ARosGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

void ARosGameState::SetGamePhase(ERosGamePhase newGamePhase)
{
	ERosGamePhase previousGamePhase = CurrentGamePhase;
	
	if(previousGamePhase != newGamePhase)
	{
		CurrentGamePhase = newGamePhase;
		
		BroadcastOnSetGamePhase();
	}
}

void ARosGameState::BroadcastOnSetGamePhase()
{
	OnSetGamePhase.Broadcast(CurrentGamePhase);
}

void ARosGameState::SetExplorationPhase()
{
	if(CurrentGamePhase == ERosGamePhase::E_ExplorationPhase)
	{
		return;
	}
	
	SetGamePhase(ERosGamePhase::E_ExplorationPhase);
	
	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	//TSubclassOf<ARosUnit> unitClass = ARosUnit::StaticClass();
	//TArray<AActor*> units;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), unitClass, units);
	//ARosUnit* defaultSelectedUnit = Cast<ARosUnit>(units[0]);

	ARosUnit* defaultSelectedUnit = controller->GetCurrentPlayerUnit();
	defaultSelectedUnit->SetGamePhase(ERosGamePhase::E_ExplorationPhase);
	

	// if(IsValid(defaultSelectedUnit->GetController()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Previous Unit Controller : %s"), *defaultSelectedUnit->GetController()->GetName());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Nothing Previous Unit Controller"));
	// }
	
	controller->UnPossess();

	// temp
	if(controller != nullptr)
	{
		controller->SetPossessCharacter(defaultSelectedUnit);
	}
}

void ARosGameState::SetBattlePhase()
{
	if(CurrentGamePhase == ERosGamePhase::E_BattlePhase)
	{
		return;
	}
	
	SetGamePhase(ERosGamePhase::E_BattlePhase);

	AddTempUnitsToNextBattleIntegration();
	DeletedBattleUnits.Empty();
	ResetBattleParameters();

	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	AddPlayerTeamToBattleTurn();
	ExecuteAddNextUnitsToBattle();
	
	controller->Possess(controller->GetTacticPawnController());

	controller->SetInputModeState(false);

	SetStartUnitTurn();
}

void ARosGameState::ResetBattleParameters()
{
	TurnsCount = 1;
	CurrentTurnIndex = 0;
	CurrentUnitsTurn.Empty();
}

void ARosGameState::AddPlayerTeamToBattleTurn()
{
	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	ARosUnit* defaultSelectedUnit = controller->GetCurrentPlayerUnit();
	controller->UnPossess();
	
	if(defaultSelectedUnit->GetUnitAiController() != nullptr)
	{
		if(defaultSelectedUnit->GetController() != defaultSelectedUnit->GetUnitAiController())
		{
			defaultSelectedUnit->GetUnitAiController()->Possess(defaultSelectedUnit);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Unit Ai Controller Found"));
	}

	controller->StopAllCompanionsState();

	for (int i = 0; i < controller->PlayerTeamUnits.Num(); ++i)
	{
		controller->PlayerTeamUnits[i]->SetGamePhase(ERosGamePhase::E_BattlePhase);
		CurrentUnitsTurn.Add(controller->PlayerTeamUnits[i]);
	}
}

void ARosGameState::AddTempUnitsToNextBattleIntegration()
{
	for (int i = 0; i < TempBattleUnits.Num(); ++i)
	{
		if(NextBattleUnitsToAdd.Contains(TempBattleUnits[i]) == false)
		{
			NextBattleUnitsToAdd.Add(TempBattleUnits[i]);
		}
	}

	TempBattleUnits.Empty();
}

void ARosGameState::ExecuteAddNextUnitsToBattle()
{
	for (int i = 0; i < NextBattleUnitsToAdd.Num(); ++i)
	{
		NextBattleUnitsToAdd[i]->SetGamePhase(ERosGamePhase::E_BattlePhase);
		CurrentUnitsTurn.Add(NextBattleUnitsToAdd[i]);
		if(CurrentTurnIndex > 0 &&
			NextBattleUnitsToAdd[i]->GetStatsComponent()->GetInitiative() > CurrentUnitsTurn[CurrentTurnIndex]->GetStatsComponent()->GetInitiative())
		{
			CurrentTurnIndex += 1;
		}
	}

	NextBattleUnitsToAdd.Empty();
}

void ARosGameState::InsertTempUnitsToBattle()
{
	AddTempUnitsToNextBattleIntegration();

	ExecuteAddNextUnitsToBattle();

	CurrentUnitsTurn.Sort(ARosUnit::InitiativeComparator);
}

void ARosGameState::NextTurn()
{
	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(controller);

	if(controller->GetMainGrid()->bAUnitMoving && controller->GetTacticPawnController()->SelectedUnit
		&& controller->GetTacticPawnController()->SelectedUnit->PathfindingMoveComponent->GetIsMoving())
	{
		controller->GetTacticPawnController()->SelectedUnit->PathfindingMoveComponent->StopPathfindingMove();
	}

	if(CurrentTurnIndex + 1 >= CurrentUnitsTurn.Num())
	{
		SetNewRoundTurn();
	}
	else
	{
		CurrentTurnIndex += 1;
	}

	SetUnitTurn(CurrentUnitsTurn[CurrentTurnIndex]);
}

void ARosGameState::SetStartUnitTurn()
{
	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	CurrentUnitsTurn.Sort(ARosUnit::InitiativeComparator);

	FVector newGridLocation = CurrentUnitsTurn[CurrentTurnIndex]->GetActorLocation() - FVector(controller->GetMainGrid()->GridSize.X / 2,
		controller->GetMainGrid()->GridSize.Y / 2, 0.0f);
	controller->GetMainGrid()->SetActorLocation(newGridLocation);
	
	controller->GetMainGrid()->ResetGrid();

	SetUnitTurn(CurrentUnitsTurn[CurrentTurnIndex]);
}

void ARosGameState::SetUnitTurn(ARosUnit* unitTurn)
{
	ARosPlayerController* controller = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(controller);

	if(controller->GetTacticPawnController()->SelectedUnit)
	{
		controller->GetTacticPawnController()->SelectedUnit->GetStateMachine()->ChangeState(EAIStates::E_NoneState);
	}
	
	controller->GetTacticPawnController()->SelectNewUnit(unitTurn);
	unitTurn->ResetTurn();

	// FVector newGridLocation = unitTurn->GetActorLocation() - FVector(controller->GetMainGrid()->GridSize.X / 2,
	// 	controller->GetMainGrid()->GridSize.Y / 2, 0.0f);
	// controller->GetMainGrid()->SetActorLocation(newGridLocation);

	// FVector2D unitTurnClosestCellPos = unitTurn->PathfindingMoveComponent->GetClosestGridCell();
	// const FS_Cell* unitTurnClosestCell = controller->GetMainGrid()->CellsMap.Find(unitTurnClosestCellPos);

	// FVector unitTurnClosestCellDir = unitTurnClosestCell->CellWorldPos - unitTurn->GetActorLocation();
	// newGridLocation = FVector(controller->GetMainGrid()->GetActorLocation().X + unitTurnClosestCellDir.X,
	// 	controller->GetMainGrid()->GetActorLocation().Y + unitTurnClosestCellDir.Y, controller->GetMainGrid()->GetActorLocation().Z);
	// controller->GetMainGrid()->SetActorLocation(newGridLocation);
	
	//controller->GetMainGrid()->ResetGrid(false);
	
	switch (unitTurn->GetCurrentBattleTeam())
	{
		case ERosBattleTeam::E_Player:
			break;
		case ERosBattleTeam::E_Ally:
			unitTurn->GetStateMachine()->ChangeState(EAIStates::E_BattleAITurnState);
			break;
		case ERosBattleTeam::E_Neutral:
			unitTurn->GetStateMachine()->ChangeState(EAIStates::E_BattleAITurnState);
			break;
		case ERosBattleTeam::E_Enemy:
			unitTurn->GetStateMachine()->ChangeState(EAIStates::E_BattleAITurnState);
			break;
		default:break;
	}
	
}

void ARosGameState::SetNewRoundTurn()
{
	CurrentTurnIndex = 0;

	CurrentUnitsTurn.Sort(ARosUnit::InitiativeComparator);
}
