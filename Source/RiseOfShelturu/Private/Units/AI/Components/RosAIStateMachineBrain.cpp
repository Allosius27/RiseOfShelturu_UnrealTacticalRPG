// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/AI/Components/RosAIStateMachineBrain.h"

#include "Units/Actors/Common/RosUnit.h"
#include "Units/AI/States/RosAIBaseState.h"

// Sets default values for this component's properties
URosAIStateMachineBrain::URosAIStateMachineBrain()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URosAIStateMachineBrain::BeginPlay()
{
	Super::BeginPlay();

	UnitOwner = Cast<ARosUnit>(GetOwner());
	
	InitialiseStates();
	ChangeState(InitialState);
}

void URosAIStateMachineBrain::InitialiseStates()
{
	for (TPair<TEnumAsByte<EAIStates>, TSubclassOf<URosAIBaseState>> state : StatesInventory)
	{
		URosAIBaseState* InitialisedState = NewObject<URosAIBaseState>(this, state.Value);
		InitialisedState->OnInitialize(GetOwner(), this);
		InitialisedStates.Add(state.Key, InitialisedState);
	}
}


// Called every frame
void URosAIStateMachineBrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentState != nullptr)
	{
		CurrentState->OnTick(DeltaTime);
	}
}

void URosAIStateMachineBrain::ChangeState(TEnumAsByte<EAIStates> NewState)
{
	if (!InitialisedStates.Contains(NewState))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "State of " + GetOwner()->GetName() + " NOT FOUND");
		}
		return;
	}
	
	if (CurrentState != nullptr)
	{
		CurrentState->OnExit();
	}
	CurrentState = InitialisedStates[NewState];
	CurrentState->OnEnter();
}

