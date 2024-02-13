// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/Player/RosPlayerController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "CommonInputTypeEnum.h"
#include "CommonInputSubsystem.h"
#include "CommonPlayerInputKey.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "System/Player/RosCheatManager.h"
#include "Units/Actors/Common/RosUnit.h"
#include "System/Player/RosTacticPawnController.h"
#include "Grid/PathfindingGrid.h"
#include "Inputs/Components/RosInputComponent.h"
#include "Inputs/Datas/RosInputConfigData.h"
#include "Kismet/GameplayStatics.h"
#include "RiseOfShelturu/RosGameplayTags.h"
#include "System/Datas/RosGameData.h"
#include "System/GameInstances/RosGameManager.h"
#include "System/GameModes/RosGameMode.h"
#include "System/GameStates/RosGameState.h"
#include "System/Player/RosPlayerStart.h"
#include "System/Player/RosPlayerState.h"
#include "UI/RosHUD.h"
#include "Units/Actors/Heroes/RosHeroUnit.h"
#include "Units/AI/Components/RosAIStateMachineBrain.h"
#include "Units/Datas/Common/RosUnitData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosPlayerController)

namespace Ros
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("RosPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

ARosPlayerController::ARosPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = false;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;
	
	DefaultMouseCursor = EMouseCursor::Default;

	TacticPawnControllerClass = ARosTacticPawnController::StaticClass();

#if USING_CHEAT_MANAGER
	CheatClass = URosCheatManager::StaticClass();
#endif // #if USING_CHEAT_MANAGER
}

void ARosPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ARosPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UE_LOG(LogTemp, Warning, TEXT("Player Controller Post Initialize Components"));

	TSubclassOf<APathfindingGrid> pathfindingGridClass = APathfindingGrid::StaticClass();
	MainGrid = Cast<APathfindingGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), pathfindingGridClass));

	MainGameState = Cast<ARosGameState>(UGameplayStatics::GetGameState(this));

}

void ARosPlayerController::BeginPlay()
{
	ARosGameMode* gameMode = Cast< ARosGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(gameMode != nullptr)
	{
		ARosPlayerStart* playerStart = Cast<ARosPlayerStart>(gameMode->FindPlayerStart(this));
		if(playerStart != nullptr)
		{
			TArray<TSubclassOf<AActor>> unitsActorsClasses;
			
			for (int i = 0; i < gameMode->GameData->PlayerTeamUnitsDatas.Num(); ++i)
			{
				unitsActorsClasses.Add(gameMode->GameData->PlayerTeamUnitsDatas[i]->PawnClass);
			}
			
			TArray<AActor*> unitsSpawned = playerStart->SpawnPlayerUnits(unitsActorsClasses);
			for (int i = 0; i < unitsSpawned.Num(); ++i)
			{
				ARosHeroUnit* heroUnit = Cast<ARosHeroUnit>(unitsSpawned[i]);
				if(heroUnit != nullptr)
				{
					PlayerTeamUnits.Add(heroUnit);
				}
			}
			
		}
	}

	if(PlayerTeamUnits.IsEmpty() == false)
	{
		CurrentPlayerUnit = PlayerTeamUnits[0];
		Possess(CurrentPlayerUnit);
	}
	
	// Call the base class  
	Super::BeginPlay();
}

void ARosPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARosPlayerController::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Controller Start Function"));
	
	SetActorHiddenInGame(false);
	
	// TSubclassOf<ARosTacticPawnController> tacticPawnControllerClass = ARosTacticPawnController::StaticClass();
	// TacticPawnController = Cast<ARosTacticPawnController>(UGameplayStatics::GetActorOfClass(GetWorld(), tacticPawnControllerClass));
	FTransform transform = GetCharacter()->GetTransform();
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.bNoFail = true;
	spawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	TacticPawnController = GetWorld()->SpawnActor<ARosTacticPawnController>(TacticPawnControllerClass, transform, spawnParams); 
	
	if(MainGrid != nullptr)
	{
		if(TacticPawnController != nullptr)
		{
			MainGrid->OnGetGridPath.AddDynamic(TacticPawnController, &ARosTacticPawnController::SetNewPathAvailable);
			MainGrid->OnSelectNewCell.AddDynamic(TacticPawnController, &ARosTacticPawnController::OnGridNewCellSelected);
			MainGrid->OnHighlightCell.AddDynamic(TacticPawnController, &ARosTacticPawnController::OnGridNewCellHighlighted);

			TacticPawnController->OnGetNewUnitSelected.AddDynamic(MainGrid, &APathfindingGrid::SetSelectedUnitComponent);
			
			MainGameState->OnSetGamePhase.AddDynamic(TacticPawnController, &ARosTacticPawnController::SetupController);
			TacticPawnController->SetupController(MainGameState->GetCurrentGamePhase());
		}
	}

	MainGameState->SetExplorationPhase();
}

void ARosPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ARosPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARosPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	// ARosUnit* RosUnit = Cast<ARosUnit>(P);
	// if (RosUnit)
	// {
	// 	RosUnit->GetAbilitySystemComponent()->InitAbilityActorInfo(RosUnit, RosUnit);
	// }
}

void ARosPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	// if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	// {
	// }

	InitializePlayerInput(InputComponent);
}

void ARosPlayerController::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Initialize Player Controller Input"));
	
	check(PlayerInputComponent);

	//UE_LOG(LogTemp, Warning, TEXT("Player Controller Has Input Component"));

	if(UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (PlayerControllerInputConfig)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Init Player Controller SubSystem Mapping"));
				
				subSystem->ClearAllMappings();
	
				subSystem->AddMappingContext(PlayerControllerInputConfig->InputsMappingContext, 0);
			
				if (const URosInputConfigData* InputConfig = PlayerControllerInputConfig)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Init Player Controller Input Component"));
					
					// The Ros Input Component has some additional functions to map Gameplay Tags to an Input Action.
					// If you want this functionality but still want to change your input component class, make it a subclass
					// of the URosInputComponent or modify this component accordingly.
					URosInputComponent* RosIC = Cast<URosInputComponent>(PlayerInputComponent);
					if (ensureMsgf(RosIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to URosInputComponent or a subclass of it.")))
					{
						// Add the key mappings that may have been set by the player
						RosIC->AddInputMappings(InputConfig, subSystem);
			
						// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
						// be triggered directly by these input actions Triggered events. 
						TArray<uint32> BindHandles;
						
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Controller_SelectCharacterSlot1, ETriggerEvent::Completed, this, &ThisClass::OnSelectCharacterSlot1, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Controller_SelectCharacterSlot2, ETriggerEvent::Completed, this, &ThisClass::OnSelectCharacterSlot2, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Controller_SelectCharacterSlot3, ETriggerEvent::Completed, this, &ThisClass::OnSelectCharacterSlot3, /*bLogIfNotFound=*/ true);
						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Controller_SelectCharacterSlot4, ETriggerEvent::Completed, this, &ThisClass::OnSelectCharacterSlot4, /*bLogIfNotFound=*/ true);

						RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Controller_SwitchCompanionsFollowState, ETriggerEvent::Completed, this, &ThisClass::OnSwitchCompanionsFollowState, /*bLogIfNotFound=*/ true);
					}
				}
			}
	}
}

void ARosPlayerController::AddAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
}

void ARosPlayerController::RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
}

void ARosPlayerController::SetPossessCharacter(ARosUnit* newUnitPossess)
{
	ARosGameState* gameState = Cast<ARosGameState>(UGameplayStatics::GetGameState(this));

	if(gameState != nullptr && gameState->GetCurrentGamePhase() != ERosGamePhase::E_ExplorationPhase)
	{
		return;
	}
	
	Possess(newUnitPossess);
	CurrentPlayerUnit = Cast<ARosHeroUnit>(newUnitPossess);
	CurrentPlayerUnit->SetCharacterPlayerPossess(true);
	SetExplorationCompanionUnitState(CurrentPlayerUnit, false);

	for (int i = 0; i < PlayerTeamUnits.Num(); ++i)
	{
		if(PlayerTeamUnits[i] != CurrentPlayerUnit)
		{
			if(PlayerTeamUnits[i]->GetUnitAiController() != nullptr)
			{
				if(PlayerTeamUnits[i]->GetController() != PlayerTeamUnits[i]->GetUnitAiController())
				{
					PlayerTeamUnits[i]->GetUnitAiController()->Possess(PlayerTeamUnits[i]);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Team Companion Unit Ai Controller Found"));
			}

			PlayerTeamUnits[i]->SetCharacterPlayerPossess(false);
			SetExplorationCompanionUnitState(PlayerTeamUnits[i], true);
		}
	}

	SetInputModeState(true);
}

void ARosPlayerController::SetExplorationCompanionUnitState(ARosUnit* unit, bool bIsAICharacter)
{
	if(bIsAICharacter)
	{
		switch (CurrentExplorationCompanionsState)
		{
			case EAIExplorationCompanionsStates::E_CompanionsFollowState:
				UE_LOG(LogTemp, Warning, TEXT("%s Set Exploration Follow Compagnon State"), *unit->GetName());
				unit->GetStateMachine()->ChangeState(EAIStates::E_ExplorationFollowState);
				break; 
			case EAIExplorationCompanionsStates::E_CompanionsWaitingState:
				UE_LOG(LogTemp, Warning, TEXT("%s Set Exploration Waiting Compagnon State"), *unit->GetName());
				unit->GetStateMachine()->ChangeState(EAIStates::E_ExplorationWaitingState);
				break;
			default: break; 
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Set Exploration Player Character State"), *unit->GetName());
		
		unit->GetStateMachine()->ChangeState(EAIStates::E_NoneState);
	}
}

void ARosPlayerController::SetAllExplorationCompanionsState()
{
	if(PlayerTeamUnits.IsEmpty())
	{
		return;
	}

	for (int i = 0; i < PlayerTeamUnits.Num(); ++i)
	{
		SetExplorationCompanionUnitState(PlayerTeamUnits[i], PlayerTeamUnits[i] != CurrentPlayerUnit);
	}
}

void ARosPlayerController::StopAllCompanionsState()
{
	for (int i = 0; i < PlayerTeamUnits.Num(); ++i)
	{
		PlayerTeamUnits[i]->GetStateMachine()->ChangeState(EAIStates::E_NoneState);
	}
}


void ARosPlayerController::SetInputModeState(bool bImmersiveGame)
{
	if(bImmersiveGame)
	{
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
}

void ARosPlayerController::OnSelectCharacterSlot(const FInputActionValue& Value, int slotIndex)
{
	ARosGameState* gameState = Cast<ARosGameState>(UGameplayStatics::GetGameState(this));

	if(gameState != nullptr && gameState->GetCurrentGamePhase() != ERosGamePhase::E_ExplorationPhase)
	{
		return;
	}

	if(PlayerTeamUnits.Num() >= slotIndex)
	{
		SetPossessCharacter(PlayerTeamUnits[slotIndex-1]);
	}
}

void ARosPlayerController::OnSelectCharacterSlot1(const FInputActionValue& Value)
{
	OnSelectCharacterSlot(Value, 1);
}

void ARosPlayerController::OnSelectCharacterSlot2(const FInputActionValue& Value)
{
	OnSelectCharacterSlot(Value, 2);
}

void ARosPlayerController::OnSelectCharacterSlot3(const FInputActionValue& Value)
{
	OnSelectCharacterSlot(Value, 3);
}

void ARosPlayerController::OnSelectCharacterSlot4(const FInputActionValue& Value)
{
	OnSelectCharacterSlot(Value, 4);
}

void ARosPlayerController::OnSwitchCompanionsFollowState(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("On Switch Companions Follow State"));
	
	switch (CurrentExplorationCompanionsState)
	{
		case EAIExplorationCompanionsStates::E_CompanionsFollowState:
			CurrentExplorationCompanionsState = E_CompanionsWaitingState;
			break; 
		case EAIExplorationCompanionsStates::E_CompanionsWaitingState:
			CurrentExplorationCompanionsState = E_CompanionsFollowState;
			break;
		default: break; 
	}

	SetAllExplorationCompanionsState();
}

ARosPlayerState* ARosPlayerController::GetRosPlayerState() const
{
	return CastChecked<ARosPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ARosHUD* ARosPlayerController::GetRosHUD() const
{
	return CastChecked<ARosHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void ARosPlayerController::OnPlayerStateChanged()
{
	// Empty, place for derived classes to implement without having to hook all the other events
}

void ARosPlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	LastSeenPlayerState = PlayerState;
}

void ARosPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ARosPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ARosPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void ARosPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
}

void ARosPlayerController::AddCheats(bool bForce)
{
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}

void ARosPlayerController::ServerCheat_Implementation(const FString& Msg)
{
#if USING_CHEAT_MANAGER
	if (CheatManager)
	{
		UE_LOG(LogRos, Warning, TEXT("ServerCheat: %s"), *Msg);
		ClientMessage(ConsoleCommand(Msg));
	}
#endif // #if USING_CHEAT_MANAGER
}

bool ARosPlayerController::ServerCheat_Validate(const FString& Msg)
{
	return true;
}

void ARosPlayerController::ServerCheatAll_Implementation(const FString& Msg)
{
#if USING_CHEAT_MANAGER
	if (CheatManager)
	{
		UE_LOG(LogRos, Warning, TEXT("ServerCheatAll: %s"), *Msg);
		for (TActorIterator<ARosPlayerController> It(GetWorld()); It; ++It)
		{
			ARosPlayerController* RosPC = (*It);
			if (RosPC)
			{
				RosPC->ClientMessage(RosPC->ConsoleCommand(Msg));
			}
		}
	}
#endif // #if USING_CHEAT_MANAGER
}

bool ARosPlayerController::ServerCheatAll_Validate(const FString& Msg)
{
	return true;
}

void ARosPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ARosPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ARosPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ARosPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARosPlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (Ros::Input::ShouldAlwaysPlayForceFeedback || CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}
	
	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void ARosPlayerController::UpdateHiddenComponents(const FVector& ViewLocation,
                                                  TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, OutHiddenComponents);
}


//////////////////////////////////////////////////////////////////////
// ARosReplayPlayerController

void ARosReplayPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
}
