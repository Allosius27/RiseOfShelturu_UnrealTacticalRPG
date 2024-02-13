// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitiesSystem/Datas/RosAbilitySetData.h"
#include "GameFramework/Character.h"
#include "System/RosSystemTypes.h"
#include "Units/Components/Common/RosStatsUnitComponent.h"
#include "Units/Datas/Common/RosUnitData.h"

#include "RosUnit.generated.h"

class URosCollisionTraceComponent;
class URosAIStateMachineBrain;
// forward declaration
class AAIController;
class AActor;
class AController;
class ARosPlayerController;	
class ARosPlayerState;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UAbilitySystemComponent;
class UInputComponent;
class URosAbilitySystemComponent;
class URosCameraComponent;
class URosStatsUnitComponent;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;
class URosUnitData;
class UOptimizationProxyComponent;
class UPathfindingMoveComponent;
class APathfindingGrid;

/**
 * ARosCharacter
 *
 *	The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class ARosUnit : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	
	ARosUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Components|Optimizaion")
	class UOptimizationProxyComponent* OptimizationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Components|Pathfinding")
	class UPathfindingMoveComponent* PathfindingMoveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Components")
	class USceneComponent* PositionPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Attributes")
	class URosMovementSet* MovementSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Attributes")
	class URosBaseStatsSet* HealthSet;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Abilities")
	// TArray<FGameplayAbilitySpecHandle> ActiveAbilities;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Abilities")
	// TArray<FGameplayAbilitySpecHandle> PassiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetIsAlive, Category = "Unit|Stats")
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Unit|GamePhase|Battle")
	TArray<FHitResult> CurrentBattleDetectHitArray;

protected:

	/** Unit data used to create the unit. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditAnywhere, Category = "Unit|Datas")
	TObjectPtr<const URosUnitData> UnitData;


	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetMainPlayerController, Category= "Unit|System")
	ARosPlayerController* MainPlayerController;

	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetStatsComponent, Category = "Unit|Stats")
	class URosStatsUnitComponent* StatsComponent;

	
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Unit|AbilitySystem")
	class URosAbilitySystemComponent* AbilitySystemComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter=GetStateMachine, Category = "Unit|StateMachine")
	URosAIStateMachineBrain* StateMachine = nullptr;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Unit|Camera")
	class URosCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCameraBoom, Category = "Unit|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCollisionTraceComponent, Category = "Unit|Components|Collision")
	URosCollisionTraceComponent* CollisionTraceComponent;
	

	UPROPERTY(BlueprintGetter=GetUnitAiController, VisibleAnywhere, Category = "Unit|Controller", meta = (DisplayPriority = 0))
	AAIController* UnitAiController;
	
	
	UPROPERTY(BlueprintGetter=GetCurrentGamePhase, VisibleAnywhere, Category = "Unit|GamePhase", meta = (DisplayPriority = 0))
	ERosGamePhase CurrentGamePhase;
	

	UPROPERTY(BlueprintGetter=GetCurrentBattleTeam, VisibleAnywhere, Category = "Unit|Team", meta = (DisplayPriority = 0))
	ERosBattleTeam CurrentBattleTeam;

	
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitializedDelegate;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitializedDelegate;


#pragma endregion

#pragma region EVENTS

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSetGamePhaseSignature OnSetGamePhase;

#pragma endregion

#pragma region FUNCTIONS

public:	

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|System")
	ARosPlayerController* GetMainPlayerController() const { return MainPlayerController; }
	
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|Components")
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|Components")
	URosCollisionTraceComponent* GetCollisionTraceComponent() const { return CollisionTraceComponent; }
	

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|Controller")
	AAIController* GetUnitAiController() const { return UnitAiController; }
	

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|GamePhase")
	ERosGamePhase GetCurrentGamePhase() const { return CurrentGamePhase; }

	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|Team")
	ERosBattleTeam GetCurrentBattleTeam() const { return CurrentBattleTeam; }

	
	UFUNCTION(BlueprintCallable, Category = "Unit|Controller")
	ARosPlayerController* GetRosPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Unit|Character")
	ARosPlayerState* GetRosPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Unit|Character")
	URosAbilitySystemComponent* GetRosAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit|Stats")
	URosStatsUnitComponent* GetStatsComponent() const { return StatsComponent; }

	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit|StateMachine")
	URosAIStateMachineBrain* GetStateMachine() const { return StateMachine; }


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Unit|Stats")
	bool GetIsAlive() const { return IsAlive; }
	

	/** Gets the unit data, which is used to specify unit properties in data */
	template <class T>
	const T* GetUnitData() const { return Cast<T>(UnitData); }

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//~End of AActor interface

	UFUNCTION(BlueprintCallable, Category = "Unit")
	void Start();

	//~APawn interface
	virtual void NotifyControllerChanged() override;
	//~End of APawn interface

	/** Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	/** Register with the OnAbilitySystemUninitialized delegate fired when our pawn is removed as the ability system's avatar actor */
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	UFUNCTION(BlueprintCallable)
	void SetAiController(AAIController* aiController);


	UFUNCTION(BlueprintCallable, Category = "Unit|GamePhase")
	virtual void SetUnitPhaseProperties(ERosGamePhase gamePhase);
	
	UFUNCTION(BlueprintCallable, Category = "Unit|GamePhase")
	void SetGamePhase(ERosGamePhase newGamePhase);

	UFUNCTION(BlueprintCallable, Category = "Unit|GamePhase")
	void BroadcastOnSetGamePhase();

	
	UFUNCTION(BlueprintCallable, Category = "Unit|Team")
	void UpdateExplorationPropertiesFromTeam();

	UFUNCTION(BlueprintCallable, Category = "Unit|Team")
	void UpdateBattlePropertiesFromTeam();

	
	static bool InitiativeComparator(const ARosUnit& unitA, const ARosUnit& unitB)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s unitA Initiative : %d - %s  unitB Initiative : %d"), *unitA.GetName(), unitA.GetStatsComponent()->GetInitiative(),
			*unitB.GetName(), unitB.GetStatsComponent()->GetInitiative());
		return unitA.GetStatsComponent()->GetInitiative() > unitB.GetStatsComponent()->GetInitiative();
	}

	UFUNCTION(BlueprintCallable)
	void ResetTurn();
	
	UFUNCTION(BlueprintCallable)
	void SelectUnit();

	UFUNCTION(BlueprintCallable)
	void UnselectUnit();

	UFUNCTION(BlueprintCallable, Category = "Unit|Grid")
	void OnPreGridReset(bool snapUnitOwner);

	UFUNCTION(BlueprintCallable, Category = "Unit|Grid")
	void OnPostGridReset(bool snapUnitOwner = true);

	UFUNCTION(BlueprintCallable, Category = "Unit|GamePhase|Battle")
	bool GetCanIntegrateBattle();

	UFUNCTION(BlueprintCallable, Category = "Unit|GamePhase|Battle")
	void SendBattleNeighbours();
	
protected:

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitializeGameplayTags();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	virtual bool CanJumpInternal_Implementation() const;

#pragma endregion

};

