// Fill out your copyright notice in the Description page of Project Settings.


#include "Feedbacks/ContextEffects/Components/RosContextEffectComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosContextEffectComponent)

// Sets default values for this component's properties
URosContextEffectComponent::URosContextEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	// ...
}


// Called when the game starts
void URosContextEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URosContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void URosContextEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

