// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "Grid/PathfindingMoveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RosTacticPawnController.generated.h"

class URosInputConfigData;
class URosTacticControllerData;
class UNiagaraSystem;
class URosCollisionTraceComponent;
class ARosPlayerController;
class URosGameManager;
class ARosUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetNewUnitSelectedSignature, UPathfindingMoveComponent*, newUnitMoveComponent);

USTRUCT()
struct FTacticInputActions
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CameraZoomAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CameraMoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CameraRotateAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SelectTargetClickAction;
};

UCLASS()
class RISEOFSHELTURU_API ARosTacticPawnController : public APawn
{
	GENERATED_BODY()

	// Sets default values for this pawn's properties
	ARosTacticPawnController();

#pragma region PROPERTIES

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TacticController")
	ARosUnit* SelectedUnit;


protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TacticController|Camera", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneRootComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TacticController|Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TacticController|Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TacticController|Components|Collision")
	URosCollisionTraceComponent* CollisionTraceComponent;

	
	UPROPERTY(EditAnywhere, Category = "TacticController|Datas")
	TObjectPtr<const URosTacticControllerData> TacticControllerData;

	
	// /** MappingContext */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "TacticController|Inputs", meta=(AllowPrivateAccess = "true"))
	// class UInputMappingContext* TacticInputsMappingContext;
	//
	// UPROPERTY(EditAnywhere, Category = "TacticController|Inputs")
	// FTacticInputActions TacticInputActions;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Location")
	float LocationInterpolation = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Location")
	float LocationSpeed = 15.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TacticController|Location")
	FVector LocationDesired;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Rotation")
	float RotationInterpolation = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Rotation")
	float RotationSpeed = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TacticController|Rotation")
	FRotator RotationDesired;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Zoom")
	float ZoomInterpolation = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Zoom")
	float ZoomSpeed = 40.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Zoom")
	float ZoomMin = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TacticController|Zoom")
	float ZoomMax = 5000.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TacticController|Zoom")
	float ZoomDesired = 700.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TacticController|Zoom")
	float DefaultZoom = 700.0f;


	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TacticController|Pathfinding")
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TacticController|Pathfinding")
	UNiagaraSystem* FXHighlightCursor;

#pragma endregion

#pragma region EVENTS

public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGetNewUnitSelectedSignature OnGetNewUnitSelected;

#pragma endregion

#pragma region FUNCTIONS

public:

	// To Call In BeginPlay Blueprint
	UFUNCTION(BlueprintCallable, Category = "TacticController")
	void Start();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "TacticController|Datas")
	void LoadTacticControllerData();

	// To Call In BeginPlay Blueprint
	UFUNCTION(BlueprintCallable, Category = "TacticController")
	void SetupController(ERosGamePhase gamePhase);

	// To Call In Tick Blueprint
	UFUNCTION(BlueprintCallable, Category = "TacticController")
	void Update(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	
	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const URosInputConfigData* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig);
	
	UFUNCTION(BlueprintCallable, Category = "TacticController")
	FHitResult LinetraceFromCamera(float distance, EDrawDebugTrace::Type drawDebugType);

	
	UFUNCTION(BlueprintCallable, Category = "TacticController|Controller")
	ARosPlayerController* GetRosPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "TacticController|GameInstance")
	URosGameManager* GetRosGameManager() const;

	
	UFUNCTION(BlueprintCallable, Category = "TacticController")
	void SelectNewUnit(ARosUnit* unit);

	UFUNCTION(BlueprintCallable, Category = "TacticController")
	void SetNewPathAvailable(const TArray<FVector2D> path);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TacticController")
	void OnTacticOverPosition(FVector worldLocation);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TacticController|Pathfinding")
	void OnGridNewCellSelected(FVector location, FVector2D cellGridLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TacticController|Pathfinding")
	void OnGridNewCellHighlighted(FVector location, FVector2D cellGridLocation);

	#pragma region INPUTS

		UFUNCTION(BlueprintCallable, Category = "TacticController|Inputs")
		void OnZoomCameraInput(const FInputActionValue& Value);

		UFUNCTION(BlueprintCallable, Category = "TacticController|Inputs")
		void OnMoveCameraInput(const FInputActionValue& Value);

		UFUNCTION(BlueprintCallable, Category = "TacticController|Inputs")
		void OnRotateCameraInput(const FInputActionValue& Value);

		UFUNCTION(BlueprintCallable, Category = "TacticController|Inputs")
		void OnSelectTargetClickInput(const FInputActionValue& Value);

		UFUNCTION(BlueprintCallable, Category = "TacticController|Inputs")
		void OnNextTurnInput(const FInputActionValue& Value);

	#pragma endregion 
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

#pragma endregion

};
