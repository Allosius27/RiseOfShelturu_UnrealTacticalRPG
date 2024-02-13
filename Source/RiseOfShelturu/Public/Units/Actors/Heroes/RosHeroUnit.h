// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/RosUnitsTypes.h"
#include "Units/Actors/Common/RosUnit.h"
#include "RosHeroUnit.generated.h"

struct FSpotPoint;
class URosContextEffectComponent;
class URosHeroComponent;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API ARosHeroUnit : public ARosUnit
{
	GENERATED_BODY()

public:
	
	ARosHeroUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	#pragma region PROPERTIES

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Components")
	URosHeroComponent* HeroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hero|Components")
	URosContextEffectComponent* ContextEffectComponent;

protected:

private:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintCallable, Category = "Hero")
	void SetCharacterPlayerPossess(bool isPlayerPossessed);
	
	
	virtual void SetUnitPhaseProperties(ERosGamePhase gamePhase) override;
	
protected:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion
};
