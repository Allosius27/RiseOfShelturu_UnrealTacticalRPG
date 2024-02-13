// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Units/RosUnitsTypes.h"
#include "RosPlayerController.generated.h"

class ARosGameState;
/** Forward declaration to improve compiling times */
class URosInputConfigData;
class ARosHeroUnit;
class ARosTacticPawnController;
class APathfindingGrid;
class ARosHUD;
class ARosPlayerState;
class APawn;
class APlayerState;
class FPrimitiveComponentId;
class IInputInterface;
class URosAbilitySystemComponent;
class UObject;
class UPlayer;
struct FFrame;

/**
 * ARosPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class ARosPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ARosPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
#pragma region PROPERTIES

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController|Datas|Input")
	TObjectPtr<URosInputConfigData> PlayerControllerInputConfig;
	
	UPROPERTY(VisibleAnywhere, Category = "PlayerController|Player")
	TArray<ARosHeroUnit*> PlayerTeamUnits;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetMainGameState, Category= "PlayerController|System")
	ARosGameState* MainGameState;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerController|Tactic")
	class TSubclassOf<class ARosTacticPawnController> TacticPawnControllerClass;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerController|Tactic")
	ARosTacticPawnController* TacticPawnController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerController|Pathfinding")
	APathfindingGrid* MainGrid;


	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCurrentPlayerUnit, Category = "PlayerController|Player")
	ARosHeroUnit* CurrentPlayerUnit;

	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "PlayerController")
	TEnumAsByte<EAIExplorationCompanionsStates> CurrentExplorationCompanionsState;

private:

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma region GETTERS

		UFUNCTION(BlueprintCallable, Category = "PlayerController")
		ARosPlayerState* GetRosPlayerState() const;

		UFUNCTION(BlueprintCallable, Category = "PlayerController")
		ARosHUD* GetRosHUD() const;

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PlayerController|System")
		ARosGameState* GetMainGameState() const {return  MainGameState;}

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PlayerController|Player")
		ARosHeroUnit* GetCurrentPlayerUnit() const {return  CurrentPlayerUnit;}
	
		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PlayerController|Tactic")
		ARosTacticPawnController* GetTacticPawnController() const {return  TacticPawnController;}

		UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PlayerController|Pathfinding")
		APathfindingGrid* GetMainGrid() const {return  MainGrid;}

	#pragma endregion

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void Start();

	//~AController interface
	virtual void OnUnPossess() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~End of APlayerController interface

	virtual void AcknowledgePossession(APawn* P) override;

	//~ACommonPlayerController interface
	virtual void OnPossess(APawn* InPawn) override;
	//~End of ACommonPlayerController interface
	
	virtual void SetupInputComponent() override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	
	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const URosInputConfigData* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig);

	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void SetPossessCharacter(ARosUnit* newUnitPossess);


	UFUNCTION(BlueprintCallable, Category = "PlayerController|AI")
	void SetExplorationCompanionUnitState(ARosUnit* unit, bool bIsAICharacter);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|AI")
	void SetAllExplorationCompanionsState();

	UFUNCTION(BlueprintCallable, Category = "PlayerController|AI")
	void StopAllCompanionsState();

#pragma region INPUTS
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void SetInputModeState(bool bImmersiveGame);
	

	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSelectCharacterSlot(const FInputActionValue& Value, int slotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSelectCharacterSlot1(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSelectCharacterSlot2(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSelectCharacterSlot3(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSelectCharacterSlot4(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|Inputs")
	void OnSwitchCompanionsFollowState(const FInputActionValue& Value);

#pragma endregion 
	
protected:

	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void AddCheats(bool bForce) override;

	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

private:

	void BroadcastOnPlayerStateChanged();

#pragma endregion

};

// A player controller used for replay capture and playback
UCLASS()
class ARosReplayPlayerController : public ARosPlayerController
{
	GENERATED_BODY()

	virtual void SetPlayer(UPlayer* InPlayer) override;
};
