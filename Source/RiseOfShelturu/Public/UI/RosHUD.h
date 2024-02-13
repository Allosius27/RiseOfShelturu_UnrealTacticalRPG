// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RosHUD.generated.h"

namespace EEndPlayReason { enum Type : int; }

class AActor;
class UObject;
	
/**
 * ARosHUD
 *
 *  Note that you typically do not need to extend or modify this class, instead you would
 *  use an "Add Widget" action in your experience to add a HUD layout and widgets to it
 * 
 *  This class exists primarily for debug rendering
 */
UCLASS(Config = Game)
class RISEOFSHELTURU_API ARosHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARosHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion 
	
protected:

	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AHUD interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
	//~End of AHUD interface

#pragma endregion
};
