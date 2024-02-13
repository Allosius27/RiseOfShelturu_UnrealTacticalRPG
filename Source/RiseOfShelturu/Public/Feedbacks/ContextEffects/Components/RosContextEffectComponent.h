// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RosContextEffectComponent.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UAudioComponent;
class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta=(BlueprintSpawnableComponent) )
class RISEOFSHELTURU_API URosContextEffectComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	URosContextEffectComponent();

#pragma region PROPERTIES

public:

private:

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma  region GETTERS

#pragma endregion 
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion
		
};
