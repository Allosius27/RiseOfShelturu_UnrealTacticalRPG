// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/AI/States/Battle/RosAIBattleTurnState.h"

#include "UtilityCustomTimer.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerController.h"
#include "System/Player/RosTacticPawnController.h"
#include "Units/Actors/Common/RosUnit.h"

URosAIBattleTurnState::URosAIBattleTurnState()
{
}

void URosAIBattleTurnState::OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner)
{
	Super::OnInitialize(Owner, BrainOwner);

	UE_LOG(LogTemp, Warning, TEXT("AI Turn State : On Initialize"));

	WaitingTurnTimer = NewObject<UUtilityCustomTimer>(UUtilityCustomTimer::StaticClass());
	WaitingTurnTimer->DispatcherOnEndTimer.AddDynamic(this, &URosAIBattleTurnState::OnEndWaitingTurn);
}

void URosAIBattleTurnState::OnEnter()
{
	Super::OnEnter();

	// temp
	WaitingTurnTimer->SetActiveTimer(true, 1.0f);
}

void URosAIBattleTurnState::OnExit()
{
	Super::OnExit();
}

void URosAIBattleTurnState::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	if(WaitingTurnTimer != nullptr)
	{
		WaitingTurnTimer->Update(DeltaTime);
	}
}

void URosAIBattleTurnState::OnEndWaitingTurn()
{
	if(UnitOwner &&  UnitOwner->GetMainPlayerController() && UnitOwner->GetMainPlayerController()->GetMainGameState())
	{
		if(UnitOwner->GetMainPlayerController()->GetTacticPawnController()->SelectedUnit == UnitOwner)
		{
			UnitOwner->GetMainPlayerController()->GetMainGameState()->NextTurn();
		}
	}
}
