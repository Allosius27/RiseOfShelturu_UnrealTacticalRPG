// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Components/Common/RosStatsUnitComponent.h"

#include "GameplayEffectExtension.h"
#include "NativeGameplayTags.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "AbilitiesSystem/Attributes/RosMovementSet.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "RiseOfShelturu/RosGameplayTags.h"
#include "System/GameInstances/RosGameManager.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerState.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Datas/Common/RosUnitData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosStatsUnitComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ros_Elimination_Message, "Ros.Elimination.Message");

// Sets default values for this component's properties
URosStatsUnitComponent::URosStatsUnitComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	UnitOwner = nullptr;
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	DeathState = ERosDeathState::NotDead;
}

void URosStatsUnitComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URosStatsUnitComponent, DeathState);
}

// Called when the game starts
void URosStatsUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void URosStatsUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URosStatsUnitComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	
	Super::OnUnregister();
}

void URosStatsUnitComponent::InitializeWithAbilitySystem(URosAbilitySystemComponent* InASC)
{
	UE_LOG(LogRos, Warning, TEXT("Initialize With Ability System"));
	
	AActor* Owner = GetOwner();
	check(Owner);

	UnitOwner = Cast<ARosUnit>(Owner);
	check(UnitOwner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	if(UnitOwner != nullptr)
	{
		HealthSet = UnitOwner->HealthSet;
		if (!HealthSet)
		{
			UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
			return;
		}

		MovementSet = UnitOwner->MovementSet;
		if (!MovementSet)
		{
			UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Cannot initialize movement component for owner [%s] with NULL movement set on the ability system."), *GetNameSafe(Owner));
			return;
		}
	}
	
	check(HealthSet);

	UE_LOG(LogRos, Warning, TEXT("Initialize Health Set"));


	// Register to listen for attribute changes.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URosBaseStatsSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URosBaseStatsSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(URosBaseStatsSet::GetHealthAttribute(), HealthSet->GetMaxHealth());

	ClearGameplayTags();

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);

	check(MovementSet);

	UE_LOG(LogRos, Warning, TEXT("Initialize Movement Set"));

	const URosUnitData* UnitData = UnitOwner->GetUnitData<URosUnitData>();
	
	AbilitySystemComponent->InitStats(URosMovementSet::StaticClass(), UnitData->AttrMovementDataTable);
	AbilitySystemComponent->InitStats(URosBaseStatsSet::StaticClass(), UnitData->AttrBaseStatsDataTable);
}

void URosStatsUnitComponent::InitAttributesValues()
{
	check(AbilitySystemComponent);

	check(UnitOwner);

	const URosUnitData* UnitData = UnitOwner->GetUnitData<URosUnitData>();

	check(UnitData);

	SetMaxHealth(UnitData->GetHealth());
	SetHealth(UnitData->GetHealth());

	SetDamage(UnitData->GetDamages());

	SetHealing(UnitData->GetHealing());

	SetMaxInitiative(UnitData->GetInitiative());
	SetInitiative(UnitData->GetInitiative());
	SetMaxMovementPoints(UnitData->GetMovementPoints());
	SetMovementPoints(UnitData->GetMovementPoints());
	SetMaxActionPoints(UnitData->GetActionPoints());
	SetActionPoints(UnitData->GetActionPoints());

	ARosGameState* gameState = Cast<ARosGameState>(UGameplayStatics::GetGameState(this));
	ERosGamePhase gamePhase = gameState->GetCurrentGamePhase();
	switch (gamePhase)
	{
		case ERosGamePhase::E_ExplorationPhase:
			SetSpeed(UnitData->GetWorldSpeed());
			break; 
		case ERosGamePhase::E_BattlePhase:
			SetSpeed(UnitData->GetTacticSpeed());
			break;
				
		default: break; 
	}
	
}

void URosStatsUnitComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();

	if (HealthSet)
	{
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
	UnitOwner = nullptr;
}

void URosStatsUnitComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(RosGameplayTags::Status_Death_Dying, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(RosGameplayTags::Status_Death_Dead, 0);
	}
}

float URosStatsUnitComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float URosStatsUnitComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float URosStatsUnitComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

float URosStatsUnitComponent::GetHealing() const
{
	return (HealthSet ? HealthSet->GetHealing() : 0.0f);
}

float URosStatsUnitComponent::GetDamages() const
{
	return (HealthSet ? HealthSet->GetDamage() : 0.0f);
}

int URosStatsUnitComponent::GetInitiative() const
{
	return (HealthSet ? HealthSet->GetInitiative() : 0.0f);
}

int URosStatsUnitComponent::GetMaxInitiative() const
{
	return (HealthSet ? HealthSet->GetMaxInitiative() : 0.0f);
}

int URosStatsUnitComponent::GetMovementPoints() const
{
	return (HealthSet ? HealthSet->GetMovementPoints() : 0.0f);
}

int URosStatsUnitComponent::GetMaxMovementPoints() const
{
	return (HealthSet ? HealthSet->GetMaxMovementPoints() : 0.0f);
}

int URosStatsUnitComponent::GetActionPoints() const
{
	return (HealthSet ? HealthSet->GetActionPoints() : 0.0f);
}

int URosStatsUnitComponent::GetMaxActionPoints() const
{
	return (HealthSet ? HealthSet->GetMaxActionPoints() : 0.0f);
}

void URosStatsUnitComponent::SetHealth(float newHealth)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetHealth(newHealth);
	}
}

void URosStatsUnitComponent::SetMaxHealth(float newMaxHealth)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetMaxHealth(newMaxHealth);
	}
}

void URosStatsUnitComponent::SetHealing(float newHealing)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetHealing(newHealing);
	}
}

void URosStatsUnitComponent::SetDamage(float newDamage)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetDamage(newDamage);
	}
}

void URosStatsUnitComponent::SetInitiative(int newInitiative)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetInitiative(newInitiative);
	}
}

void URosStatsUnitComponent::SetMaxInitiative(int newMaxInitiative)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetMaxInitiative(newMaxInitiative);
	}
}

void URosStatsUnitComponent::SetMovementPoints(int newMovementPoints)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetMovementPoints(newMovementPoints);
	}
}

void URosStatsUnitComponent::SetMaxMovementPoints(int newMaxMovementPoints)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetMaxMovementPoints(newMaxMovementPoints);
	}
}

void URosStatsUnitComponent::SetActionPoints(int newActionPoints)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetActionPoints(newActionPoints);
	}
}

void URosStatsUnitComponent::SetMaxActionPoints(int newMaxActionPoints)
{
	if (IsValid(HealthSet))
	{
		HealthSet->SetMaxActionPoints(newMaxActionPoints);
	}
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

	return nullptr;
}

void URosStatsUnitComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

void URosStatsUnitComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

void URosStatsUnitComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		{
			FGameplayEventData Payload;
			Payload.EventTag = RosGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec.Def;
			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

		// Send a standardized verb message that other systems can observe
		{
		}
	}

#endif // #if WITH_SERVER_CODE
}

void URosStatsUnitComponent::OnRep_DeathState(ERosDeathState OldDeathState)
{
	const ERosDeathState NewDeathState = DeathState;

	// Revert the death state for now since we rely on StartDeath and FinishDeath to change it.
	DeathState = OldDeathState;

	if (OldDeathState > NewDeathState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogRos, Warning, TEXT("RosStatsUnitComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDeathState == ERosDeathState::NotDead)
	{
		if (NewDeathState == ERosDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == ERosDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDeathState == ERosDeathState::DeathStarted)
	{
		if (NewDeathState == ERosDeathState::DeathFinished)
		{
			FinishDeath();
		}
		else
		{
			UE_LOG(LogRos, Error, TEXT("RosStatsUnitComponent: Invalid death transition [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DeathState == NewDeathState), TEXT("RosStatsUnitComponent: Death transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDeathState, (uint8)NewDeathState, *GetNameSafe(GetOwner()));
}

void URosStatsUnitComponent::SetSpeed(float newSpeed)
{
	if (IsValid(MovementSet))
	{
		MovementSet->SetSpeed(newSpeed);
	}
}

void URosStatsUnitComponent::StartDeath()
{
	if (DeathState != ERosDeathState::NotDead)
	{
		return;
	}

	DeathState = ERosDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(RosGameplayTags::Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void URosStatsUnitComponent::FinishDeath()
{
	if (DeathState != ERosDeathState::DeathStarted)
	{
		return;
	}

	DeathState = ERosDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(RosGameplayTags::Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void URosStatsUnitComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if ((DeathState == ERosDeathState::NotDead) && AbilitySystemComponent)
	{
	}
}

