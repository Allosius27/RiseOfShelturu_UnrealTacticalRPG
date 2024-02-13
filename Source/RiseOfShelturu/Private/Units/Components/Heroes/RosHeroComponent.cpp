// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Components/Heroes/RosHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitiesSystem/Abilities/Common/RosGameplayAbilityJump.h"
#include "AbilitiesSystem/Components/RosAbilitySystemComponent.h"
#include "Inputs/Components/RosInputComponent.h"
#include "RiseOfShelturu/RosGameplayTags.h"
#include "System/Player/RosLocalPlayer.h"
#include "System/Player/RosPlayerController.h"
#include "System/Player/RosPlayerState.h"
#include "Units/Actors/Common/RosUnit.h"
#include "Units/Datas/Common/RosUnitData.h"
#include "Units/Datas/Heroes/RosHeroData.h"

#include <float.h>

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

URosHeroComponent::URosHeroComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void URosHeroComponent::OnRegister()
{
	Super::OnRegister();
}

// Called when the game starts
void URosHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RosUnitOwner = Cast<ARosUnit>(GetOwner());

	LoadHeroData();

	ZoomDesired = RosUnitOwner->GetCameraBoom()->TargetArmLength;
	DefaultZoom = ZoomDesired;
}

void URosHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URosHeroComponent::LoadHeroData()
{
	if(!RosUnitOwner)
	{
		RosUnitOwner = Cast<ARosUnit>(GetOwner());
	}
	
	const URosHeroData* HeroData = RosUnitOwner->GetUnitData<URosHeroData>();
	
	check(HeroData);

	ZoomInterpolation = HeroData->GetZoomInterpolation();
	ZoomSpeed = HeroData->GetZoomSpeed();
	ZoomMin = HeroData->GetZoomMin();
	ZoomMax = HeroData->GetZoomMax();
}

// Called every frame
void URosHeroComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(RosUnitOwner != nullptr)
	{
		RosUnitOwner->GetCameraBoom()->TargetArmLength = UKismetMathLibrary::FInterpTo(RosUnitOwner->GetCameraBoom()->TargetArmLength, ZoomDesired, GetWorld()->GetDeltaSeconds(), ZoomInterpolation);
	}
}

void URosHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Start Initialize Player Input"));
	
	check(PlayerInputComponent);

	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = Pawn->GetController<APlayerController>();
	check(PC);

	const URosLocalPlayer* LP = Cast<URosLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if(!RosUnitOwner)
	{
		RosUnitOwner = Cast<ARosUnit>(GetOwner());
	}
	
	if(RosUnitOwner)
	{
		const URosHeroData* HeroData = RosUnitOwner->GetUnitData<URosHeroData>();
		
		if (HeroData)
		{
			Subsystem->ClearAllMappings();

			Subsystem->AddMappingContext(HeroData->InputConfig->InputsMappingContext, 0);
			
			if (const URosInputConfigData* InputConfig = HeroData->InputConfig)
			{
				// The Ros Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the URosInputComponent or modify this component accordingly.
				URosInputComponent* RosIC = Cast<URosInputComponent>(PlayerInputComponent);
				if (ensureMsgf(RosIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to URosInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					RosIC->AddInputMappings(InputConfig, Subsystem);
			
					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					RosIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
			
					RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput, /*bLogIfNotFound=*/ false);
					RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::OnLookMouseInput, /*bLogIfNotFound=*/ false);
					RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::OnJumpInput, /*bLogIfNotFound=*/ false);
					RosIC->BindNativeAction(InputConfig, RosGameplayTags::InputTag_Zoom, ETriggerEvent::Triggered, this, &ThisClass::OnZoomInput, false);
				}
			}
		}
	}
}

void URosHeroComponent::AddAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = Pawn->GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	URosInputComponent* RosIC = Pawn->FindComponentByClass<URosInputComponent>();
	if (ensureMsgf(RosIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to URosInputComponent or a subclass of it.")))
	{
		RosIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
}

void URosHeroComponent::RemoveAdditionalInputConfig(const URosInputConfigData* InputConfig)
{
	//@TODO: Implement me!
}

void URosHeroComponent::DrawSpotPointsDebugGizmos()
{
	for (int i = 0; i < CharacterSpotsPoints.Num(); ++i)
	{
		DrawDebugSphere(GetWorld(),  GetOwner()->GetActorLocation() + CharacterSpotsPoints[i].SpotPointPosition, 20, 16, CharacterSpotsPoints[i].DebugColor, false, 5, 0, 2);
	}
}

int URosHeroComponent::GetFreeTargetClosestSpotPointIndex(FVector targetLocation, bool& hasFoundSpotPoint)
{
	hasFoundSpotPoint = false;
	float closestDistance = FLT_MAX;
	int closestSpotPointIndex = 0;
	
	if(CharacterSpotsPoints.IsEmpty() == false)
	{
		int currentClosestSpotPointPriority = CharacterSpotsPoints[0].Priority;
		
		for (int i = 0; i < CharacterSpotsPoints.Num(); ++i)
		{
			if(CharacterSpotsPoints[i].Priority < currentClosestSpotPointPriority)
			{
				if(hasFoundSpotPoint)
				{
					break;
				}
				
				currentClosestSpotPointPriority = CharacterSpotsPoints[i].Priority;
			}
			
			if(CharacterSpotsPoints[i].IsTaken == false)
			{
				float distance = FVector::Dist(GetOwner()->GetActorLocation() + CharacterSpotsPoints[i].SpotPointPosition, targetLocation);
				if(distance <= closestDistance)
				{
					closestSpotPointIndex = i;
					closestDistance = distance;
					hasFoundSpotPoint = true;
				}
			}
		}
	}

	return  closestSpotPointIndex;
}

void URosHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		const ARosUnit* RosUnit = Cast<ARosUnit>(Pawn);
		if (URosAbilitySystemComponent* RosASC = RosUnit->GetRosAbilitySystemComponent())
		{
			RosASC->AbilityInputTagPressed(InputTag);
		}	
	}
}

void URosHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	if(!RosUnitOwner)
	{
		RosUnitOwner = Cast<ARosUnit>(GetOwner());
	}
	
	if (URosAbilitySystemComponent* RosASC = RosUnitOwner->GetRosAbilitySystemComponent())
	{
		RosASC->AbilityInputTagReleased(InputTag);
	}
}

void URosHeroComponent::OnMoveInput(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Move Input"));

	APawn* Pawn = Cast<APawn>(GetOwner());
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
			//UE_LOG(LogTemp, Warning, TEXT("On Move Input %f"), Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
			//UE_LOG(LogTemp, Warning, TEXT("On Move Input %f"), Value.Y);
		}
	}
}

void URosHeroComponent::OnLookMouseInput(const FInputActionValue& InputActionValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Look Mouse Input"));
	
	APawn* Pawn = Cast<APawn>(GetOwner());

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void URosHeroComponent::OnJumpInput(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump Input"));
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	
	if(!RosUnitOwner)
	{
		RosUnitOwner = Cast<ARosUnit>(GetOwner());
	}
	
	if (URosAbilitySystemComponent* RosASC = RosUnitOwner->GetRosAbilitySystemComponent())
	{
		const URosUnitData* UnitData = RosUnitOwner->GetUnitData<URosUnitData>();
		if(UnitData != nullptr)
		{
			RosASC->TryActivateAbilitiesByTag(UnitData->AbilityTagsContainers->JumpAbilityTagContainer);
		}
	}
}

void URosHeroComponent::OnZoomInput(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom Input"));
	
	float actionValue = InputActionValue[0];
	ZoomDesired = UKismetMathLibrary::FClamp((actionValue * ZoomSpeed) + ZoomDesired, ZoomMin, ZoomMax);
}




