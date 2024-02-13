// Copyright Epic Games, Inc. All Rights Reserved.

#include "Units/Actors/Common/RosUnit.h"

#include "AIController.h"
#include "OptimizationComponent.h"
#include "AbilitiesSystem/Attributes/RosBaseStatsSet.h"
#include "AbilitiesSystem/Attributes/RosMovementSet.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Camera/RosCameraComponent.h"
#include "Collision/Components/RosCollisionTraceComponent.h"
#include "Components/CapsuleComponent.h"
#include "System/Player/RosPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grid/PathfindingGrid.h"
#include "Grid/PathfindingMoveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/GameInstances/RosGameManager.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerState.h"
#include "Units/AI/Components/RosAIStateMachineBrain.h"
#include "Units/Components/Common/RosCharacterMovementComponent.h"
#include "Units/Components/Common/RosStatsUnitComponent.h"
#include "Units/Datas/Common/RosUnitData.h"

class AActor;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UInputComponent;

static FName NAME_RosCharacterCollisionProfile_Capsule(TEXT("RosPawn"));
static FName NAME_RosCharacterCollisionProfile_Mesh(TEXT("RosPawnMesh"));
static FName NAME_RosNpcCollisionProfile_Capsule(TEXT("RosNpcPawn"));
static FName NAME_RosBattlePawnCollisionProfile_Capsule(TEXT("RosBattlePawn"));

ARosUnit::ARosUnit(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<URosCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_RosCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_RosCharacterCollisionProfile_Mesh);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	URosCharacterMovementComponent* RosMoveComp = CastChecked<URosCharacterMovementComponent>(GetCharacterMovement());
	RosMoveComp->SetWalkableFloorAngle(90.0f);
	RosMoveComp->MaxWalkSpeed = 900.0f;
	RosMoveComp->MaxWalkSpeedCrouched = 450.0f;
	RosMoveComp->GravityScale = 1.0f;
	RosMoveComp->MaxAcceleration = 2400.0f;
	RosMoveComp->BrakingFrictionFactor = 1.0f;
	RosMoveComp->BrakingFriction = 6.0f;
	RosMoveComp->GroundFriction = 8.0f;
	RosMoveComp->BrakingDecelerationWalking = 1400.0f;
	RosMoveComp->bUseControllerDesiredRotation = false;
	RosMoveComp->bOrientRotationToMovement = false;
	RosMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	RosMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	// RosMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	// RosMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	// RosMoveComp->SetCrouchedHalfHeight(65.0f);

	StatsComponent = CreateDefaultSubobject<URosStatsUnitComponent>(TEXT("StatsComponent"));
	StatsComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	StatsComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	StateMachine = CreateDefaultSubobject<URosAIStateMachineBrain>(TEXT("StateMachine"));
	AddOwnedComponent(StateMachine);

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URosAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	MovementSet = ObjectInitializer.CreateDefaultSubobject<URosMovementSet>(this, TEXT("MovementSet"));
	HealthSet = ObjectInitializer.CreateDefaultSubobject<URosBaseStatsSet>(this, TEXT("HealthSet"));

	//OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	//OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 550.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = true;
	

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<URosCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->SetRelativeLocation(FVector(4.0f, 0.0f, 47.0f));
	CameraComponent->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
	CameraComponent->SetFieldOfView(90.0f);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	PositionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PosPoint"));
	PositionPoint->SetupAttachment(RootComponent);
	PositionPoint->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	OptimizationComponent = CreateDefaultSubobject<UOptimizationProxyComponent>(TEXT("OptimizationComponent"));
	
	PathfindingMoveComponent = CreateDefaultSubobject<UPathfindingMoveComponent>(TEXT("PathfindingMoveComponent"));

	CollisionTraceComponent = CreateDefaultSubobject<URosCollisionTraceComponent>(TEXT("CollisionTraceComponent"));
}

void ARosUnit::PreInitializeComponents()
{
	//UE_LOG(LogTemp, Warning, TEXT("Pre Initialize Components"));
	
	Super::PreInitializeComponents();

	if(IsValid(MovementSet) == false)
	{
		MovementSet = NewObject<URosMovementSet>(this, TEXT("MovementSet"), RF_Transient);
		MovementSet->SetNetAddressable();
	}
	
	if(IsValid(HealthSet) == false)
	{
		HealthSet = NewObject<URosBaseStatsSet>(this, TEXT("HealthSet"), RF_Transient);
		HealthSet->SetNetAddressable();
	}
}

void ARosUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	MainPlayerController = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARosUnit::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("Begin Play"));
	
	Super::BeginPlay();

	if(MainPlayerController != nullptr)
	{
		MainPlayerController->GetMainGrid()->OnPreResetGrid.AddDynamic(this, &ARosUnit::OnPreGridReset);
		MainPlayerController->GetMainGrid()->OnPostResetGrid.AddDynamic(this, &ARosUnit::OnPostGridReset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Main Player Controller Is Null"));
	}
}

void ARosUnit::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnAbilitySystemUninitialized();
}

void ARosUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARosUnit::Start()
{
	UE_LOG(LogRos, Warning, TEXT("Unit Start"));

	CurrentBattleTeam = UnitData->GetBattleTeam();

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if(CapsuleComp)
	{
		switch (CurrentBattleTeam)
		{
			case ERosBattleTeam::E_Player:
				break;
			case ERosBattleTeam::E_Ally:
				CapsuleComp->SetCollisionProfileName(NAME_RosNpcCollisionProfile_Capsule);
				break;
			case ERosBattleTeam::E_Neutral:
				CapsuleComp->SetCollisionProfileName(NAME_RosNpcCollisionProfile_Capsule);
				break;
			case ERosBattleTeam::E_Enemy:
				CapsuleComp->SetCollisionProfileName(NAME_RosNpcCollisionProfile_Capsule);
				break;
			default:break;
		}
		
	}

	
	OnAbilitySystemInitialized();
	
	PathfindingMoveComponent->InitComponent(PositionPoint);

	FTransform transform = GetTransform();
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.bNoFail = true;
	spawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SetAiController(GetWorld()->SpawnActor<AAIController>(AIControllerClass, transform, spawnParams));

	if(CurrentBattleTeam != ERosBattleTeam::E_Player)
	{
		GetUnitAiController()->Possess(this);
	}

	OnSetGamePhase.AddDynamic(this, &ARosUnit::SetUnitPhaseProperties);
	if(GetRosPlayerState() != nullptr)
	{
		ARosGameState* gameState = Cast<ARosGameState>(UGameplayStatics::GetGameState(this));
		SetUnitPhaseProperties(gameState->GetCurrentGamePhase());
	}
	else
	{
		SetUnitPhaseProperties(ERosGamePhase::E_ExplorationPhase);
	}
}

void ARosUnit::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

void ARosUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARosUnit::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
}


void ARosUnit::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

void ARosUnit::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitializedDelegate.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitializedDelegate.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void ARosUnit::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitializedDelegate.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitializedDelegate.Add(Delegate);
	}
}

void ARosUnit::SetAiController(AAIController* aiController)
{
	UnitAiController = aiController;
}

void ARosUnit::SetUnitPhaseProperties(ERosGamePhase gamePhase)
{
	UE_LOG(LogRos, Warning, TEXT("Set Unit Phase Properties"));

	if(MainPlayerController == nullptr)
	{
		MainPlayerController = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	
	URosCharacterMovementComponent* RosMoveComp = CastChecked<URosCharacterMovementComponent>(GetCharacterMovement());
	switch (gamePhase)
	{
		case ERosGamePhase::E_ExplorationPhase:
			
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = false;
			bUseControllerRotationRoll = false;
		
			RosMoveComp->bOrientRotationToMovement = true;

			PathfindingMoveComponent->ResetUnitOnCurrentCell();
			// MainPlayerController->GetMainGrid()->OnPreResetGrid.RemoveDynamic(this, &ARosUnit::OnPreGridReset);
			// MainPlayerController->GetMainGrid()->OnPostResetGrid.RemoveDynamic(this, &ARosUnit::OnPostGridReset);

			StatsComponent->SetSpeed(UnitData->GetWorldSpeed());

			UpdateExplorationPropertiesFromTeam();
		
			break; 
		case ERosGamePhase::E_BattlePhase:
			
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = true;
			bUseControllerRotationRoll = false;
		
			RosMoveComp->bOrientRotationToMovement = false;

			PathfindingMoveComponent->SetClosestGridCell();
			// MainPlayerController->GetMainGrid()->OnPreResetGrid.AddDynamic(this, &ARosUnit::OnPreGridReset);
			// MainPlayerController->GetMainGrid()->OnPostResetGrid.AddDynamic(this, &ARosUnit::OnPostGridReset);

			StatsComponent->SetSpeed(UnitData->GetTacticSpeed());

			UpdateBattlePropertiesFromTeam();
		
			break;
			
		default: break; 
	}
}

void ARosUnit::SetGamePhase(ERosGamePhase newGamePhase)
{
	ERosGamePhase previousGamePhase = CurrentGamePhase;
	
	CurrentGamePhase = newGamePhase;

	if(previousGamePhase != CurrentGamePhase)
	{
		BroadcastOnSetGamePhase();
	}
}

void ARosUnit::BroadcastOnSetGamePhase()
{
	OnSetGamePhase.Broadcast(CurrentGamePhase);
}

void ARosUnit::UpdateExplorationPropertiesFromTeam()
{
	switch (CurrentBattleTeam)
	{
		case ERosBattleTeam::E_Player:

		break;

		case ERosBattleTeam::E_Ally:

		break;

		case ERosBattleTeam::E_Neutral:

			break;

		case ERosBattleTeam::E_Enemy:

			this->GetStateMachine()->ChangeState(EAIStates::E_ExplorationBattleDetectState);
			
			break;

		default: break;
	}
}

void ARosUnit::UpdateBattlePropertiesFromTeam()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if(CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(NAME_RosBattlePawnCollisionProfile_Capsule);
	}
	
	switch (CurrentBattleTeam)
	{
		case ERosBattleTeam::E_Player:

			break;

		case ERosBattleTeam::E_Ally:

			break;

		case ERosBattleTeam::E_Neutral:

			break;

		case ERosBattleTeam::E_Enemy:
			
			this->GetStateMachine()->ChangeState(EAIStates::E_NoneState);
				
			break;

		default: break;
	}
}


ARosPlayerController* ARosUnit::GetRosPlayerController() const
{
	return CastChecked<ARosPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ARosPlayerState* ARosUnit::GetRosPlayerState() const
{
	return CastChecked<ARosPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

URosAbilitySystemComponent* ARosUnit::GetRosAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* ARosUnit::GetAbilitySystemComponent() const
{
	return GetRosAbilitySystemComponent();
}

void ARosUnit::OnAbilitySystemInitialized()
{
	UE_LOG(LogRos, Warning, TEXT("Unit On Ability System Initialize"));
	
	URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent();
	check(RosASC);

	UE_LOG(LogRos, Warning, TEXT("Unit Initialize With Ability System Stats Component"));
	
	StatsComponent->InitializeWithAbilitySystem(RosASC);
	
	InitializeGameplayTags();

	for (const URosAbilitySetData* AbilitySet : UnitData->ActiveAbilitySets)
	{
		if (AbilitySet)
		{
			FRosAbilitySet_GrantedHandles OutActiveGrantedHandles;
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &OutActiveGrantedHandles);
			UE_LOG(LogRos, Warning, TEXT("ActiveAbilitiesGrantedHandles : %d"), OutActiveGrantedHandles.AbilitySpecHandles.Num());
			// for (int i = 0; i < OutActiveGrantedHandles.AbilitySpecHandles.Num(); ++i)
			// {
			// 	ActiveAbilities.AddUnique(OutActiveGrantedHandles.AbilitySpecHandles[i]);
			// }
		}
	}

	for (const URosAbilitySetData* AbilitySet : UnitData->PassiveAbilitySets)
	{
		if (AbilitySet)
		{
			FRosAbilitySet_GrantedHandles OutPassiveGrantedHandles;
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &OutPassiveGrantedHandles, true);
			UE_LOG(LogRos, Warning, TEXT("PassiveAbilitiesGrantedHandles : %d"), OutPassiveGrantedHandles.AbilitySpecHandles.Num());
			// for (int i = 0; i < OutPassiveGrantedHandles.AbilitySpecHandles.Num(); ++i)
			// {
			// 	PassiveAbilities.AddUnique(OutPassiveGrantedHandles.AbilitySpecHandles[i]);
			// }
		}
	}

	StatsComponent->InitAttributesValues();
}

void ARosUnit::OnAbilitySystemUninitialized()
{
	StatsComponent->UninitializeFromAbilitySystem();
}

void ARosUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

		
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}

void ARosUnit::UnPossessed()
{
	Super::UnPossessed();
}

void ARosUnit::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void ARosUnit::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

// Called to bind functionality to input
void ARosUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARosUnit::InitializeGameplayTags()
{
	
}

void ARosUnit::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent())
	{
		RosASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ARosUnit::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent())
	{
		return RosASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ARosUnit::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent())
	{
		return RosASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ARosUnit::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const URosAbilitySystemComponent* RosASC = GetRosAbilitySystemComponent())
	{
		return RosASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void ARosUnit::FellOutOfWorld(const UDamageType& dmgType)
{
	StatsComponent->DamageSelfDestruct(/*bFellOutOfWorld=*/ true);
}

void ARosUnit::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ARosUnit::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ARosUnit::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	URosCharacterMovementComponent* RosMoveComp = CastChecked<URosCharacterMovementComponent>(GetCharacterMovement());
	RosMoveComp->StopMovementImmediately();
	RosMoveComp->DisableMovement();
}

void ARosUnit::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ARosUnit::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	SetActorHiddenInGame(true);
}

bool ARosUnit::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}

void ARosUnit::ResetTurn()
{
	StatsComponent->SetInitiative(StatsComponent->GetMaxInitiative());
	StatsComponent->SetMovementPoints(StatsComponent->GetMaxMovementPoints());
	StatsComponent->SetActionPoints(StatsComponent->GetMaxActionPoints());
}

void ARosUnit::SelectUnit()
{
	UE_LOG(LogRos, Warning, TEXT("Selected"));
}

void ARosUnit::UnselectUnit()
{
	UE_LOG(LogRos, Warning, TEXT("UnSelected"));
}

void ARosUnit::OnPreGridReset(bool snapUnitOwner)
{
	PathfindingMoveComponent->ResetUnitOnCurrentCell();
}

void ARosUnit::OnPostGridReset(bool snapUnitOwner)
{
	PathfindingMoveComponent->SetClosestGridCell(snapUnitOwner);
}

bool ARosUnit::GetCanIntegrateBattle()
{
	return MainPlayerController->GetMainGameState()->TempBattleUnits.Contains(this) == false &&
		CurrentGamePhase != ERosGamePhase::E_BattlePhase;
}

void ARosUnit::SendBattleNeighbours()
{
	UE_LOG(LogRos, Warning, TEXT("SendBattleNeighbours %s"), *GetName());
	if(GetCanIntegrateBattle())
	{
		UE_LOG(LogRos, Warning, TEXT("Add Unit %s To TempBattleUnits"), *GetName());
		MainPlayerController->GetMainGameState()->TempBattleUnits.Add(this);
	}

	TArray<FHitResult> battleDetectHitArray = CurrentBattleDetectHitArray;
	for(const FHitResult& result : battleDetectHitArray)
	{
		ARosUnit* unit = Cast<ARosUnit>(result.GetActor());
	
		if(unit && unit->GetCurrentBattleTeam() != ERosBattleTeam::E_Player)
		{
			if(unit->GetCanIntegrateBattle())
			{
				MainPlayerController->GetMainGameState()->TempBattleUnits.Add(unit);
				unit->SendBattleNeighbours();
			}
		}
	}
}

