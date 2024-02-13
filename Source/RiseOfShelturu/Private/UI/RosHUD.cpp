// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RosHUD.h"

ARosHUD::ARosHUD(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ARosHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARosHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ARosHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARosHUD::GetDebugActorList(TArray<AActor*>& InOutList)
{
	Super::GetDebugActorList(InOutList);
}
