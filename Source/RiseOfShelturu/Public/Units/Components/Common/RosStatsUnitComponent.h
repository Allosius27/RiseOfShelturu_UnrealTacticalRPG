// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Units/RosUnitsTypes.h"
#include "RosStatsUnitComponent.generated.h"

class URosStatsUnitComponent;

class URosBaseStatsSet;
class URosAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayEffectSpec;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRosUnitStatsDeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRosUnitStatsAttributeChanged, URosStatsUnitComponent*, StatsComponent, float, OldValue, float, NewValue, AActor*, Instigator);


/**
 * URosHealthComponent
 *
 *	An actor component used to handle anything related to health.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class RISEOFSHELTURU_API URosStatsUnitComponent : public UActorComponent
{
	GENERATED_BODY()

	// Sets default values for this component's properties
	URosStatsUnitComponent(const FObjectInitializer& ObjectInitializer);

#pragma region PROPERTIES

public:

protected:
	
	// Health set used by this component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARosUnit* UnitOwner;
	
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<URosAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URosBaseStatsSet* HealthSet;

	// Movement set used by this component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URosMovementSet* MovementSet;

	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	ERosDeathState DeathState;

#pragma endregion

#pragma region EVENTS

public:

	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FRosUnitStatsAttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed.
	UPROPERTY(BlueprintAssignable)
	FRosUnitStatsAttributeChanged OnMaxHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FRosUnitStatsDeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FRosUnitStatsDeathEvent OnDeathFinished;

#pragma endregion

#pragma region FUNCTIONS

public:

#pragma  region GETTERS

#pragma endregion

	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "StatsComponent")
	static URosStatsUnitComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<URosStatsUnitComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "StatsComponent")
	void InitializeWithAbilitySystem(URosAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent")
	void InitAttributesValues();

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "StatsComponent")
	void UninitializeFromAbilitySystem();

	
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	float GetHealing() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	float GetDamages() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetInitiative() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetMaxInitiative() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetMovementPoints() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetMaxMovementPoints() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetActionPoints() const;

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	int GetMaxActionPoints() const;
	
	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	virtual void SetHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	virtual void SetMaxHealth(float newMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	virtual void SetHealing(float newHealing);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	virtual void SetDamage(float newDamage);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetInitiative(int newInitiative);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetMaxInitiative(int newMaxInitiative);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetMovementPoints(int newMovementPoints);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetMaxMovementPoints(int newMaxMovementPoints);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetActionPoints(int newActionPoints);

	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Battle")
	virtual void SetMaxActionPoints(int newMaxActionPoints);

	
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Health")
	ERosDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatsComponent|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > ERosDeathState::NotDead); }


	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Speed")
	virtual void SetSpeed(float newSpeed);
	
	
	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	virtual void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);

	UFUNCTION()
	virtual void OnRep_DeathState(ERosDeathState OldDeathState);

#pragma endregion
		
};
