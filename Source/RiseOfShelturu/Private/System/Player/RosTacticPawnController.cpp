// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/RosTacticPawnController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Collision/Components/RosCollisionTraceComponent.h"
#include "System/Player/RosPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "System/GameInstances/RosGameManager.h"
#include "Grid/PathfindingGrid.h"
#include "Grid/PathfindingMoveComponent.h"
#include "Inputs/Components/RosInputComponent.h"
#include "Inputs/Datas/RosInputConfigData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RiseOfShelturu/RosGameplayTags.h"
#include "System/Datas/RosTacticControllerData.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosLocalPlayer.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Actors/Heroes/RosHeroUnit.h"

// Sets default values
ARosTacticPawnController::ARosTacticPawnController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate pawn to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneRootComponent->SetupAttachment(RootComponent);

	// Create a camera boom...
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SceneRootComponent);
	SpringArmComponent->TargetArmLength = 2000.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	CameraComponent->FieldOfView = 55.0f;
	CameraComponent->bConstrainAspectRatio = false;
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CollisionTraceComponent = CreateDefaultSubobject<URosCollisionTraceComponent>(TEXT("CollisionTraceComponent"));
}

// Called when the game starts or when spawned
void ARosTacticPawnController::BeginPlay()
{
	Super::BeginPlay();

	//[EN] Set the input context
	//[FR] Applique le input context
	// if(APlayerController* playerController = Cast<APlayerController>(GetController()))
	// {
	// 	if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
	// 	{
	// 		subSystem->ClearAllMappings();
	//
	// 		subSystem->AddMappingContext(TacticControllerData->InputConfig->InputsMappingContext, 0);
	// 	}
	// }
}

void ARosTacticPawnController::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ARosTacticPawnController::UnPossessed()
{
	Super::UnPossessed();
}

void ARosTacticPawnController::Start()
{
	LoadTacticControllerData();
	
	// [EN] At the start of the game, we keep a snapshot fo the current state of the camera as our starting point
	// [FR] Au début du jeu, on prend en note l'état actuel de notre caméra pour s'en servir comme point de départ
	LocationDesired = GetActorLocation();
	RotationDesired = GetActorRotation();
	ZoomDesired = SpringArmComponent->TargetArmLength;
	DefaultZoom = ZoomDesired;
}

// Called every frame
void ARosTacticPawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARosTacticPawnController::LoadTacticControllerData()
{
	check(TacticControllerData);

	LocationInterpolation = TacticControllerData->GetLocationInterpolation();
	LocationSpeed = TacticControllerData->GetLocationSpeed();

	RotationInterpolation = TacticControllerData->GetRotationInterpolation();
	RotationSpeed = TacticControllerData->GetRotationSpeed();

	ZoomInterpolation = TacticControllerData->GetZoomInterpolation();
	ZoomSpeed = TacticControllerData->GetZoomSpeed();
	ZoomMin = TacticControllerData->GetZoomMin();
	ZoomMax = TacticControllerData->GetZoomMax();
}

void ARosTacticPawnController::SetupController(ERosGamePhase gamePhase)
{
	ARosPlayerController* playerController = Cast<ARosPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(playerController);

	UE_LOG(LogRos, Warning, TEXT("Setup Controller"));
	
	switch (gamePhase)
	{
		case ERosGamePhase::E_ExplorationPhase:
			SetActorHiddenInGame(true);
			if(playerController != nullptr && playerController->GetMainGrid() != nullptr)
			{
				playerController->GetMainGrid()->ResetPath();
			}
			break;
		case ERosGamePhase::E_BattlePhase:
			SetActorHiddenInGame(false);
			// temp
			//SelectNewUnit(defaultSelectedUnit);
			break;
		default: break; 
	}
}

void ARosTacticPawnController::Update(float DeltaTime)
{
	ARosPlayerController* playerController = GetRosPlayerController();
	if(SelectedUnit != nullptr && SelectedUnit->GetCurrentBattleTeam() == ERosBattleTeam::E_Player)
	{
		if(playerController != nullptr && playerController->GetMainGrid() != nullptr)
		{
			if(playerController->GetMainGrid()->bAUnitMoving == false)
			{
				FVector2D currentSelectedUnitCellPos = SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex();
				
				FHitResult hitResult = LinetraceFromCamera(40000.0f, EDrawDebugTrace::None);
				ARosUnit* hitUnit = Cast<ARosUnit>(hitResult.GetActor());
				if(hitUnit)
				{
					FVector2D cellUnitPos = hitUnit->PathfindingMoveComponent->GetCurrentGridIndex();
					const FS_Cell* cellUnitFound = playerController->GetMainGrid()->CellsMap.Find(cellUnitPos);
					if(cellUnitFound && cellUnitFound->PathfindingUnitComponentRef)
					{
						//UE_LOG(LogRos, Warning, TEXT("Over Detect Unit Cell"));
						bool successCellFound = false;
						FVector2D closestCellNeighbourPos = playerController->GetMainGrid()->GetClosestCellNeighbour(cellUnitPos, currentSelectedUnitCellPos, successCellFound);
						const FS_Cell* cellNeighbourTarget = playerController->GetMainGrid()->CellsMap.Find(closestCellNeighbourPos);
						if(cellNeighbourTarget && SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != closestCellNeighbourPos
							&& successCellFound)
						{
							playerController->GetMainGrid()->OverNewCell(closestCellNeighbourPos);
							OnTacticOverPosition(cellNeighbourTarget->CellWorldPos);
						}
					}
				}
				else
				{
					FVector2D cellPosFounded = APathfindingGrid::FindGridCell(hitResult.ImpactPoint, playerController->GetMainGrid());
					const FS_Cell* cellFound = playerController->GetMainGrid()->CellsMap.Find(cellPosFounded);
					if(cellFound && cellFound->PathfindingUnitComponentRef)
					{
						//UE_LOG(LogRos, Warning, TEXT("Over Detect Cell With Unit"));
						bool successCellFound = false;
						FVector2D closestCellNeighbourPos = playerController->GetMainGrid()->GetClosestCellNeighbour(cellPosFounded, currentSelectedUnitCellPos, successCellFound);
						const FS_Cell* cellNeighbourTarget = playerController->GetMainGrid()->CellsMap.Find(closestCellNeighbourPos);
						if(cellNeighbourTarget && SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != closestCellNeighbourPos
							&& successCellFound)
						{
							playerController->GetMainGrid()->OverNewCell(closestCellNeighbourPos);
							OnTacticOverPosition(cellNeighbourTarget->CellWorldPos);
						}
					}
					else
					{
						//UE_LOG(LogRos, Warning, TEXT("Over Detect Simple Cell"));
						if(SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != cellPosFounded)
						{
							playerController->GetMainGrid()->OverNewCell(cellPosFounded);
							OnTacticOverPosition(hitResult.ImpactPoint);
						}
					}
				}
			}
		}
	}

	if(playerController != nullptr && playerController->GetMainGrid() != nullptr)
	{
		if(playerController->GetMainGrid()->bAUnitMoving)
		{
			if(SelectedUnit != nullptr)
			{
				LocationDesired = SelectedUnit->GetActorLocation();
			}
		}
	}

	//[EN] Every frame, we interpolate the camera movements data (to make it smoother) and apply them to the actual camera
	//[FR] À chaque frame, on interpole le data du mouvement de la caméra (pour le rendre plus smooth) et l'applique sur la vrai caméra
	SpringArmComponent->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, ZoomDesired, GetWorld()->GetDeltaSeconds(), ZoomInterpolation);
	SetActorLocation(UKismetMathLibrary::VInterpTo(GetActorLocation(), LocationDesired, GetWorld()->GetDeltaSeconds(), LocationInterpolation));
	SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), RotationDesired, GetWorld()->GetDeltaSeconds(), RotationInterpolation));
}

// Called to bind functionality to input
void ARosTacticPawnController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializePlayerInput(PlayerInputComponent);
}

void ARosTacticPawnController::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Start Initialize Tactic Player Input"));
	
	check(PlayerInputComponent);

	//UE_LOG(LogTemp, Warning, TEXT("Tactic Player Has Input Component"));

	if(APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			if (TacticControllerData)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Init Tactic Player SubSystem Mapping"));
				
				subSystem->ClearAllMappings();
	
				subSystem->AddMappingContext(TacticControllerData->InputConfig->InputsMappingContext, 0);
			
				if (const URosInputConfigData* InputConfig = TacticControllerData->InputConfig)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Init Tactic Input Component"));
					
					// The Ros Input Component has some additional functions to map Gameplay Tags to an Input Action.
					// If you want this functionality but still want to change your input component class, make it a subclass
					// of the URosInputComponent or modify this component accordingly.
					URosInputComponent* RosIC = Cast<URosInputComponent>(PlayerInputComponent);
					if (ensureMsgf(RosIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to URosInputComponent or a subclass of it.")))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Tactic Input Components Binds"));
				
						// Add the key mappings that may have been set by the player
						RosIC->AddInputMappings(InputConfig, subSystem);
			
						// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
						// be triggered directly by these input actions Triggered events. 
						TArray<uint32> BindHandles;
			
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Tactic_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnZoomCameraInput, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Tactic_Move_Camera, ETriggerEvent::Triggered, this, &ThisClass::OnMoveCameraInput, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Tactic_Rotate_Camera, ETriggerEvent::Triggered, this, &ThisClass::OnRotateCameraInput, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Tactic_SelectTarget, ETriggerEvent::Completed, this, &ThisClass::OnSelectTargetClickInput, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Tactic_NextTurn, ETriggerEvent::Completed, this, &ThisClass::OnNextTurnInput, /*bLogIfNotFound=*/ true);
					}
				}
			}
		}
	}
}

void ARosTacticPawnController::AddAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
	//@TODO: Implement me!
}

void ARosTacticPawnController::RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
	//@TODO: Implement me!
}

FHitResult ARosTacticPawnController::LinetraceFromCamera(float distance, EDrawDebugTrace::Type drawDebugType)
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	
	FVector start = CameraComponent->GetComponentLocation();
	
	FVector mouseWorldLocation;
	FVector mouseWorldDirection;
	bool bconvertMousePositionToWorld = playerController->DeprojectMousePositionToWorld(mouseWorldLocation, mouseWorldDirection);

	FVector end = CameraComponent->GetComponentLocation() + (mouseWorldDirection * distance);
	
	FHitResult hit;
	
	CollisionTraceComponent->LineTraceByChannel(start, end, ECC_Visibility, drawDebugType, hit);

	return hit;
}

ARosPlayerController* ARosTacticPawnController::GetRosPlayerController() const
{
	return CastChecked<ARosPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

URosGameManager* ARosTacticPawnController::GetRosGameManager() const
{
	return CastChecked<URosGameManager>(GetWorld()->GetGameInstance(), ECastCheckedType::NullAllowed);
}

void ARosTacticPawnController::SelectNewUnit(ARosUnit* unit)
{
	if(SelectedUnit != nullptr)
	{
		SelectedUnit->UnselectUnit();
		SelectedUnit = unit;
	}
	else
	{
		SelectedUnit = unit;
	}

	if(SelectedUnit == nullptr)
	{
		return;
	}
	
	SelectedUnit->SelectUnit();

	OnGetNewUnitSelected.Broadcast(SelectedUnit->PathfindingMoveComponent);

	LocationDesired = SelectedUnit->GetActorLocation();
	RotationDesired = FRotator(0.0f, 0.0f, 0.0f);
	ZoomDesired = DefaultZoom;
}

void ARosTacticPawnController::SetNewPathAvailable(const TArray<FVector2D> path)
{
	if(SelectedUnit != nullptr)
	{
		SelectedUnit->PathfindingMoveComponent->SetPathfindingPath(path);
	}
}

void ARosTacticPawnController::OnZoomCameraInput(const FInputActionValue& Value)
{
	//[EN] Every frame, we're adding the input amount to the Zoom_Desired. Make sure to clamp the result so the player can't zoom all the way to the moon.
	//[FR] À chaque frame, on additionne la valeur de l'input au Zoom_Desired. N'oublie pas de clamp le résultat pour ne pas que le joueur puisse se rendre jusqu'à la lune.
	float actionValue = Value[0];
	ZoomDesired = UKismetMathLibrary::FClamp((actionValue * ZoomSpeed) + ZoomDesired, ZoomMin, ZoomMax);
}

void ARosTacticPawnController::OnMoveCameraInput(const FInputActionValue& Value)
{
	//[EN] Every frame, we're adding the input amount to the Location_Desired. Make sure to multiply it by the Forward & Right Vectors so the camera moves in the right direction.
   //[FR] À chaque frame, on additionne la valeur de l'input au Location_Desired. N'oublie pas de multiplier par le Forward & Right Vectors pour que la caméra se déplace dans le bonne direction
	ARosPlayerController* playerController = GetRosPlayerController();
	if(playerController != nullptr && playerController->GetMainGrid() != nullptr)
	{
		if(playerController->GetMainGrid()->bAUnitMoving == false)
		{
			const FVector2D MoveValue = Value.Get<FVector2D>();
			LocationDesired = (GetActorForwardVector() * (UKismetMathLibrary::Normal2D(MoveValue).X * LocationSpeed)) + LocationDesired +
				(GetActorRightVector() * (UKismetMathLibrary::Normal2D(MoveValue).Y * LocationSpeed));
			URosGameManager* gameManager = GetRosGameManager();
			// if(GEngine && gameManager != nullptr && gameManager->GetDrawDebugAllow())
			// {
			// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Location Desired : X = %f, Y = %f, Z = %f"), LocationDesired.X, LocationDesired.Y, LocationDesired.Z));
			// }
		}
	}
}

void ARosTacticPawnController::OnRotateCameraInput(const FInputActionValue& Value)
{
	//[EN] Every time the player presses the inputs, we're adding the input amount to the Rotation_Desired.
	//[FR] À chaque fois que le joueur appuie sur les inputs, on additionne la valeur de l'input au Rotation_Desired.
	ARosPlayerController* playerController = GetRosPlayerController();
	if(playerController != nullptr && playerController->GetMainGrid() != nullptr)
	{
		if(playerController->GetMainGrid()->bAUnitMoving == false)
		{
			RotationDesired = UKismetMathLibrary::ComposeRotators(RotationDesired, FRotator(0.0f, Value[0] * RotationSpeed, 0.0f));
		}
	}
}

void ARosTacticPawnController::OnSelectTargetClickInput(const FInputActionValue& Value)
{
	if(SelectedUnit != nullptr && SelectedUnit->GetCurrentBattleTeam() == ERosBattleTeam::E_Player)
	{
		FHitResult hitResult = LinetraceFromCamera(40000.0f, EDrawDebugTrace::None);
		ARosPlayerController* playerController = GetRosPlayerController();
		if(playerController != nullptr)
		{
			FVector2D currentSelectedUnitCellPos = SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex();
			
			ARosUnit* hitUnit = Cast<ARosUnit>(hitResult.GetActor());
			if(hitUnit)
			{
				FVector2D cellUnitPos = hitUnit->PathfindingMoveComponent->GetCurrentGridIndex();
				UE_LOG(LogRos, Warning, TEXT("Target Unit Cell : X : %f Y : %f"), cellUnitPos.X, cellUnitPos.Y);
				const FS_Cell* cellUnitFound = playerController->GetMainGrid()->CellsMap.Find(cellUnitPos);
				if(cellUnitFound->PathfindingUnitComponentRef)
				{
					//UE_LOG(LogRos, Warning, TEXT("Click Detect Unit Cell"));
					bool successCellFound = false;
					FVector2D closestCellNeighbourPos = playerController->GetMainGrid()->GetClosestCellNeighbour(cellUnitPos, currentSelectedUnitCellPos, successCellFound, true);
					if(SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != closestCellNeighbourPos && successCellFound)
					{
						UE_LOG(LogRos, Warning, TEXT("Closest Cell : X : %f Y : %f"), closestCellNeighbourPos.X, closestCellNeighbourPos.Y);
						
						UE_LOG(LogRos, Warning, TEXT("Select New Cell With Unit"));
						playerController->GetMainGrid()->SelectNewCell(closestCellNeighbourPos);
					}
				}
			}
			else
			{
				FVector2D cellPosFounded = APathfindingGrid::FindGridCell(hitResult.ImpactPoint, playerController->GetMainGrid());
				UE_LOG(LogRos, Warning, TEXT("Target Unit Cell : X : %f Y : %f"), cellPosFounded.X, cellPosFounded.Y);
				const FS_Cell* cellFound = playerController->GetMainGrid()->CellsMap.Find(cellPosFounded);
				if(cellFound->PathfindingUnitComponentRef)
				{
					//UE_LOG(LogRos, Warning, TEXT("Click Detect Cell With Unit"));
					bool successCellFound = false;
					FVector2D closestCellNeighbourPos = playerController->GetMainGrid()->GetClosestCellNeighbour(cellPosFounded, currentSelectedUnitCellPos, successCellFound, true);
					if(SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != closestCellNeighbourPos && successCellFound)
					{
						UE_LOG(LogRos, Warning, TEXT("Closest Cell : X : %f Y : %f"), closestCellNeighbourPos.X, closestCellNeighbourPos.Y);
					
						UE_LOG(LogRos, Warning, TEXT("Select New Cell With Unit"));
						playerController->GetMainGrid()->SelectNewCell(closestCellNeighbourPos);
					}
				}
				else
				{
					//UE_LOG(LogRos, Warning, TEXT("Click Detect Simple Cell"));
					if(SelectedUnit->PathfindingMoveComponent->GetCurrentGridIndex() != cellPosFounded)
					{
						UE_LOG(LogRos, Warning, TEXT("Cell : X : %f Y : %f"), cellPosFounded.X, cellPosFounded.Y);

						UE_LOG(LogRos, Warning, TEXT("Select New Simple Cell"));
						playerController->GetMainGrid()->SelectNewCell(cellPosFounded);
					}
				}
			}
		}
		
	}
}

void ARosTacticPawnController::OnNextTurnInput(const FInputActionValue& Value)
{
	ARosPlayerController* playerController = GetRosPlayerController();
	check(playerController);

	if(playerController->GetMainGameState()->GetCurrentGamePhase() == ERosGamePhase::E_BattlePhase
		&& SelectedUnit->GetCurrentBattleTeam() == ERosBattleTeam::E_Player)
	{
		playerController->GetMainGameState()->NextTurn();
	}
}

