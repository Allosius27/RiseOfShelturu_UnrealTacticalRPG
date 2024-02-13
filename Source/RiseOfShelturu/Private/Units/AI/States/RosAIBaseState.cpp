// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/AI/States/RosAIBaseState.h"

#include "Kismet/GameplayStatics.h"
#include "System/Player/RosPlayerController.h"
#include "Units/Actors/Common/RosUnit.h"

URosAIBaseState::URosAIBaseState()
{
}

void URosAIBaseState::OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner)
{
	StateMachineBrainOwner = BrainOwner;
	ActorOwner = Owner;

	UnitOwner = Cast<ARosUnit>(Owner);
}

void URosAIBaseState::OnEnter()
{
	CurrentPlayerController = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void URosAIBaseState::OnExit()
{
}

void URosAIBaseState::OnTick(float DeltaTime)
{
}
