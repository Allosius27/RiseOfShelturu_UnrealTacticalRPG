// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/RosPlayerState.h"

#include "AIController.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "System/GameInstances/RosGameManager.h"
#include "System/Player/RosPlayerController.h"
#include "System/Player/RosTacticPawnController.h"
#include "Units/Actors/Common/RosUnit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosPlayerState)

class AController;
class APlayerState;
class FLifetimeProperty;

const FName ARosPlayerState::NAME_RosAbilityReady("RosAbilitiesReady");

ARosPlayerState::ARosPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetUpdateFrequency = 100.0f;
}

void ARosPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARosPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARosPlayerState::Reset()
{
	Super::Reset();
}

void ARosPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
}

void ARosPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void ARosPlayerState::OnDeactivated()
{
	Super::OnDeactivated();
}

void ARosPlayerState::OnReactivated()
{
	Super::OnReactivated();
}

void ARosPlayerState::OnRep_UnitData()
{
}

void ARosPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, UnitData, SharedParams);
}

ARosPlayerController* ARosPlayerState::GetRosPlayerController() const
{
	return Cast<ARosPlayerController>(GetOwner());
}

void ARosPlayerState::SetUnitData(const URosUnitData* InUnitData)
{
	check(InUnitData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (UnitData)
	{
		UE_LOG(LogRos, Error, TEXT("Trying to set UnitData on player state that already has valid UnitData."));
		return;
	}
	
	UnitData = InUnitData;
	
	ForceNetUpdate();
}
