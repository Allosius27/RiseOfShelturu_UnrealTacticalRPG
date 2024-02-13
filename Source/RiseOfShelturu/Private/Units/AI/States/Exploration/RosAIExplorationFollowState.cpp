// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/AI/States/Exploration/RosAIExplorationFollowState.h"

#include "AIController.h"
#include "UtilityCustomTimer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/Player/RosPlayerController.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Actors/Heroes/RosHeroUnit.h"
#include "Units/AI/Components/RosAIStateMachineBrain.h"
#include "Units/Components/Heroes/RosHeroComponent.h"

URosAIExplorationFollowState::URosAIExplorationFollowState()
{
}

void URosAIExplorationFollowState::OnInitialize(AActor* Owner, URosAIStateMachineBrain* BrainOwner)
{
	Super::OnInitialize(Owner, BrainOwner);

	UE_LOG(LogTemp, Warning, TEXT("Follow State : On Intialize"));

	MoveCooldownTimer = NewObject<UUtilityCustomTimer>(UUtilityCustomTimer::StaticClass());
	MoveCooldownTimer->DispatcherOnStartTimer.AddDynamic(this, &URosAIExplorationFollowState::OnStartMove);
	MoveCooldownTimer->DispatcherOnEndTimer.AddDynamic(this, &URosAIExplorationFollowState::OnEndMoveWaiting);
}

void URosAIExplorationFollowState::OnEnter()
{
	Super::OnEnter();

	UE_LOG(LogTemp, Warning, TEXT("Follow State : On Enter"));

	if (CurrentPlayerController)
	{
		ARosHeroUnit* newTargetActor = CurrentPlayerController->GetCurrentPlayerUnit();
		if(newTargetActor != TargetUnit)
		{
			if(TargetUnit != nullptr && HasTargetSpotPoint)
			{
				TargetUnit->HeroComponent->CharacterSpotsPoints[TargetSpotPointIndex].IsTaken = false;
				HasTargetSpotPoint = false;
				TargetSpotPointIndex = 0;
			}
				
			TargetUnit = newTargetActor;
		}
			
		if (TargetUnit)
		{
				bool hasFoundSpotPoint = false;
				int spotPointIndex = TargetUnit->HeroComponent->GetFreeTargetClosestSpotPointIndex(StateMachineBrainOwner->GetUnitOwner()->GetActorLocation(),
					hasFoundSpotPoint);
				
				if(hasFoundSpotPoint)
				{
					if(HasTargetSpotPoint)
					{
						TargetUnit->HeroComponent->CharacterSpotsPoints[TargetSpotPointIndex].IsTaken = false;
					}
					
					TargetSpotPointIndex = spotPointIndex;
					HasTargetSpotPoint = true;
					TargetUnit->HeroComponent->CharacterSpotsPoints[TargetSpotPointIndex].IsTaken = true;

					UE_LOG(LogTemp, Warning, TEXT("%s Find Spot Point To Follow : %d"), *StateMachineBrainOwner->GetUnitOwner()->GetName(), TargetSpotPointIndex);
				}
		}
	}

	OnEndMoveWaiting();
}

void URosAIExplorationFollowState::OnExit()
{
	Super::OnExit();

	//UE_LOG(LogTemp, Warning, TEXT("Follow State : On Exit"));
	
	if(StateMachineBrainOwner != nullptr && StateMachineBrainOwner->GetUnitOwner() != nullptr)
	{
		AAIController* AIController = Cast<AAIController>(StateMachineBrainOwner->GetUnitOwner()->GetUnitAiController());
	
		if (AIController)
		{
			AIController->StopMovement();
	
			//UE_LOG(LogTemp, Warning, TEXT("Follow State : Stop Movement"));
		}

		if(TargetUnit != nullptr && HasTargetSpotPoint)
		{
			TargetUnit->HeroComponent->CharacterSpotsPoints[TargetSpotPointIndex].IsTaken = false;
			HasTargetSpotPoint = false;
			TargetSpotPointIndex = 0;

			TargetUnit = nullptr;
		}
	}
}

void URosAIExplorationFollowState::OnTick(float DeltaTime)
{
	Super::OnTick(DeltaTime);
	
	if(MoveCooldownTimer != nullptr)
	{
		MoveCooldownTimer->Update(DeltaTime);
	
		//UE_LOG(LogTemp, Warning, TEXT("Follow State : Move Cooldown Timer Update"));
	}
}

void URosAIExplorationFollowState::OnStartMove()
{
	//UE_LOG(LogTemp, Warning, TEXT("Follow State : On Start Move"));
	
	if(StateMachineBrainOwner != nullptr && StateMachineBrainOwner->GetUnitOwner() != nullptr)
	{
		AAIController* AIController = Cast<AAIController>(StateMachineBrainOwner->GetUnitOwner()->GetUnitAiController());
	
		if (AIController && CurrentPlayerController)
		{
			if (TargetUnit)
			{
				if(HasTargetSpotPoint)
				{
					FVector targetLocation = TargetUnit->GetActorLocation() +
						TargetUnit->HeroComponent->CharacterSpotsPoints[TargetSpotPointIndex].SpotPointPosition;
					float distance = FVector::Dist(StateMachineBrainOwner->GetUnitOwner()->GetActorLocation(), targetLocation);
					if(distance > StateMachineBrainOwner->GetExplorationFollowMinimalDistanceToMove())
					{
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, targetLocation);

						UE_LOG(LogTemp, Warning, TEXT("%s Follow State : Move To Location : %d"), *StateMachineBrainOwner->GetUnitOwner()->GetName(), TargetSpotPointIndex);
					}
				}
				else
				{
					// float distance = FVector::Dist(StateMachineBrainOwner->GetUnitOwner()->GetActorLocation(), TargetUnit->GetActorLocation());
					// if(distance > StateMachineBrainOwner->GetExplorationFollowMinimalDistanceToMove())
					// {
					// 	UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, TargetUnit);
					//
					// 	UE_LOG(LogTemp, Warning, TEXT("%s Follow State : Move To Actor"), *StateMachineBrainOwner->GetUnitOwner()->GetName());
					// }
				}
			}
		}
	}
}

void URosAIExplorationFollowState::OnEndMoveWaiting()
{
	//UE_LOG(LogTemp, Warning, TEXT("Follow State : On End Move Waiting"));

	float rnd = FMath::FRandRange(0, StateMachineBrainOwner->GetExplorationFollowWaitingTime() / 4);
	float duration = StateMachineBrainOwner->GetExplorationFollowWaitingTime() + rnd;
	MoveCooldownTimer->SetActiveTimer(true, duration);
}
