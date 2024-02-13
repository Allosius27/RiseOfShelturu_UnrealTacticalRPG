// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Units/RosUnitsTypes.h"
#include "RosHeroComponent.generated.h"

class ARosUnit;

namespace EEndPlayReason { enum Type : int; }
struct FLoadedMappableConfigPair;
struct FMappableConfigPair;

class UInputComponent;
class URosInputConfigData;
class UObject;
struct FActorInitStateChangedParams;
struct FFrame;
struct FGameplayTag;
struct FInputActionValue;
/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RISEOFSHELTURU_API URosHeroComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	URosHeroComponent(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroComponent")
	TArray<FRosUnitSpotPoint> CharacterSpotsPoints {};

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroComponent")
	ARosUnit* RosUnitOwner;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroComponent|Zoom")
	float ZoomInterpolation = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroComponent|Zoom")
	float ZoomSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroComponent|Zoom")
	float ZoomMin = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroComponent|Zoom")
	float ZoomMax = 800.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HeroComponent|Zoom")
	float ZoomDesired = 550.0f;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetDefaultZoom, Category = "HeroComponent|Zoom")
	float DefaultZoom = 550.0f;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroComponent")
	float GetDefaultZoom() const { return DefaultZoom; }

#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "HeroComponent|Datas")
	void LoadHeroData();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "HeroComponent")
	static URosHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URosHeroComponent>() : nullptr); }

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	
	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const URosInputConfigData* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "HeroComponent")
	void DrawSpotPointsDebugGizmos();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "HeroComponent")
	int GetFreeTargetClosestSpotPointIndex(FVector targetLocation, bool& hasFoundSpotPoint);

	UFUNCTION(BlueprintCallable, Category = "HeroComponent")
	void SetZoomDesired(float value)
	{
		ZoomDesired = value;
	}
	
protected:
	
	virtual void OnRegister() override;
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable, Category = "HeroComponent|Inputs")
	void OnMoveInput(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category = "HeroComponent|Inputs")
	void OnLookMouseInput(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category = "HeroComponent|Inputs")
	void OnJumpInput(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable, Category = "HeroComponent|Inputs")
	void OnZoomInput(const FInputActionValue& InputActionValue);

#pragma endregion
		
};
