// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "System/RosSystemTypes.h"
#include "RosPlayerState.generated.h"

class AController;
class ARosPlayerController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class URosAbilitySystemComponent;
class URosUnitData;
class UObject;
struct FFrame;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class RISEOFSHELTURU_API ARosPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARosPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

	static const FName NAME_RosAbilityReady;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_UnitData)
	TObjectPtr<const URosUnitData> UnitData;

private:

#pragma endregion

#pragma region EVENTS

public:

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma  region GETTERS

		UFUNCTION(BlueprintCallable, Category = "PlayerState")
		ARosPlayerController* GetRosPlayerController() const;

		template <class T>
		const T* GetUnitData() const { return Cast<T>(UnitData); }

	#pragma endregion

	void SetUnitData(const URosUnitData* InUnitData);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface
	
	
protected:

	UFUNCTION()
	void OnRep_UnitData();

#pragma endregion
};
