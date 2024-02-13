// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Actors/Heroes/RosHeroUnit.h"

#include "Components/CapsuleComponent.h"
#include "Feedbacks/ContextEffects/Components/RosContextEffectComponent.h"
#include "Units/Components/Heroes/RosHeroComponent.h"

static FName NAME_RosPlayerCollisionProfile_Capsule(TEXT("RosPlayerPawn"));
static FName NAME_RosCompanionCollisionProfile_Capsule(TEXT("RosCompanionPawn"));

ARosHeroUnit::ARosHeroUnit(const FObjectInitializer& ObjectInitializer)
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(NAME_RosCompanionCollisionProfile_Capsule);
	
	HeroComponent = CreateDefaultSubobject<URosHeroComponent>(TEXT("HeroComponent"));

	ContextEffectComponent = CreateDefaultSubobject<URosContextEffectComponent>(TEXT("ContextEffectComponent"));
}

void ARosHeroUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HeroComponent->CharacterSpotsPoints.Sort(FRosUnitSpotPoint::PriorityComparator);
}

void ARosHeroUnit::SetCharacterPlayerPossess(bool isPlayerPossessed)
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);

	if(isPlayerPossessed)
	{
		CapsuleComp->SetCollisionProfileName(NAME_RosPlayerCollisionProfile_Capsule);
	}
	else
	{
		CapsuleComp->SetCollisionProfileName(NAME_RosCompanionCollisionProfile_Capsule);
	}

	HeroComponent->SetZoomDesired(HeroComponent->GetDefaultZoom());
}

void ARosHeroUnit::SetUnitPhaseProperties(ERosGamePhase gamePhase)
{
	Super::SetUnitPhaseProperties(gamePhase);

	// switch (gamePhase)
	// {
	// 	case ERosGamePhase::E_ExplorationPhase:
	// 			
	// 		
	// 		
	// 		break; 
	// 	case ERosGamePhase::E_BattlePhase:
	// 			
	// 		
	// 		
	// 		break;
	// 			
	// 	default: break; 
	// }
}

void ARosHeroUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	HeroComponent->InitializePlayerInput(PlayerInputComponent);
}
