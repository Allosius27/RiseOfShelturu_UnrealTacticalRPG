// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/AI/States/Exploration/RosExplorationBattleDetectState.h"

#include "Collision/Components/RosCollisionTraceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerController.h"
#include "Units/Actors/Heroes/RosHeroUnit.h"
#include "Units/AI/Components/RosAIStateMachineBrain.h"

URosExplorationBattleDetectState::URosExplorationBattleDetectState()
{
}

void URosExplorationBattleDetectState::OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner)
{
	Super::OnInitialize(Owner, BrainOwner);

	CanDetect = true;
}

void URosExplorationBattleDetectState::OnEnter()
{
	Super::OnEnter();

	if(UnitOwner == nullptr)
	{
		UnitOwner = Cast<ARosHeroUnit>(ActorOwner);
	}

	CanDetect = true;
}

void URosExplorationBattleDetectState::OnExit()
{
	Super::OnExit();

	if(UnitOwner)
	{
		UnitOwner->CurrentBattleDetectHitArray.Empty();
	}
}

void URosExplorationBattleDetectState::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);

	if(UnitOwner && StateMachineBrainOwner && CanDetect)
	{
		if(UnitOwner->IsAlive && UnitOwner->GetCurrentGamePhase() == ERosGamePhase::E_ExplorationPhase)
		{
			FVector pos = UnitOwner->GetActorLocation();
			TArray<FHitResult> OutHits;
			UnitOwner->CurrentBattleDetectHitArray.Empty();
			bool hit = false;
			if(UnitOwner->GetMainPlayerController()->GetMainGameState()->GetCurrentGamePhase() != ERosGamePhase::E_BattlePhase)
			{
				hit = UnitOwner->GetCollisionTraceComponent()->SphereTraceMulti(pos, pos, StateMachineBrainOwner->GetBattleDetectionRange(),
				StateMachineBrainOwner->BattleDetectionObjectTypes, OutHits);
				if (hit && UnitOwner->GetMainPlayerController() && UnitOwner->GetMainPlayerController()->GetMainGameState())
				{
					UnitOwner->CurrentBattleDetectHitArray = OutHits;
					for(const FHitResult& result : OutHits)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%s Detection %s"), *GetActorOwner()->GetName(), *result.GetActor()->GetName());
					
						ARosHeroUnit* heroUnit = Cast<ARosHeroUnit>(result.GetActor());
	
						if(heroUnit)
						{
							OnBattleDetectHero(heroUnit, true);
						}
					}
				}
			}
			else
			{
				hit = UnitOwner->GetCollisionTraceComponent()->SphereTraceMulti(pos, pos, StateMachineBrainOwner->GetBattleDetectionRange() * 2,
				StateMachineBrainOwner->BattleDetectionObjectTypes, OutHits);
				if (hit && UnitOwner->GetMainPlayerController() && UnitOwner->GetMainPlayerController()->GetMainGameState())
				{
					UnitOwner->CurrentBattleDetectHitArray.Empty();
					UnitOwner->CurrentBattleDetectHitArray = OutHits;
					for(const FHitResult& result : OutHits)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s Detection %s"), *GetActorOwner()->GetName(), *result.GetActor()->GetName());
					
						ARosHeroUnit* heroUnit = Cast<ARosHeroUnit>(result.GetActor());
			
						if(heroUnit)
						{
							OnBattleDetectHero(heroUnit, false);
						}
					}
				}
			}
			

			
		}
	}
}

void URosExplorationBattleDetectState::OnBattleDetectHero(ARosHeroUnit* hero, bool canLaunchBattle)
{
	if(hero == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s On Battle Detect Hero : %s"), *GetActorOwner()->GetName(), *hero->GetName());
	
	switch (UnitOwner->GetCurrentBattleTeam())
	{
		case ERosBattleTeam::E_Player:
								
			break; 
		case ERosBattleTeam::E_Ally:
				
			break;
		case ERosBattleTeam::E_Neutral:
					
			break;
		case ERosBattleTeam::E_Enemy:
								
			if(hero && CanDetect)
			{
				if(UnitOwner->GetMainPlayerController()->PlayerTeamUnits.Contains(hero))
				{
					if(UnitOwner->GetMainPlayerController()->GetMainGameState()->GetCurrentGamePhase() != ERosGamePhase::E_BattlePhase)
					{
						if(canLaunchBattle)
						{
							UE_LOG(LogTemp, Warning, TEXT("Enemy Launch Battle State With %s"), *hero->GetName());

							CanDetect = false;

							UnitOwner->SendBattleNeighbours();
						
							UnitOwner->GetMainPlayerController()->GetMainGameState()->SetBattlePhase();
						}
					}
					else
					{
						if(hero->GetCurrentGamePhase() == ERosGamePhase::E_BattlePhase)
						{
							UE_LOG(LogTemp, Warning, TEXT("Enemy Integrates Battle With Detecting %s"), *hero->GetName());

							CanDetect = false;

							UnitOwner->SendBattleNeighbours();

							UnitOwner->GetMainPlayerController()->GetMainGameState()->InsertTempUnitsToBattle();
						}
					}
				}
				else
				{
					if(UnitOwner->GetMainPlayerController()->GetMainGameState()->GetCurrentGamePhase() == ERosGamePhase::E_BattlePhase)
					{
						if(hero->GetCurrentGamePhase() == ERosGamePhase::E_BattlePhase)
						{
							UE_LOG(LogTemp, Warning, TEXT("Enemy Integrates Battle On Detect %s"), *hero->GetName());

							CanDetect = false;

							UnitOwner->SendBattleNeighbours();

							UnitOwner->GetMainPlayerController()->GetMainGameState()->InsertTempUnitsToBattle();
						}
					}
				}
			}
								
			break;
					
		default: break; 
	}
}
