// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/Components/RosCollisionTraceComponent.h"

#include "System/GameInstances/RosGameManager.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
URosCollisionTraceComponent::URosCollisionTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URosCollisionTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URosCollisionTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

URosGameManager* URosCollisionTraceComponent::GetRosGameManager() const
{
	return CastChecked<URosGameManager>(GetWorld()->GetGameInstance(), ECastCheckedType::NullAllowed);
}

bool URosCollisionTraceComponent::LineTraceByChannel(FVector start, FVector end, ECollisionChannel TraceChannel,
	EDrawDebugTrace::Type drawDebugType , FHitResult& OutHits)
{
	FVector StartLocation = start; // L'emplacement de départ de la trace
	FVector EndLocation = end; // L'emplacement de fin de la trace

	EDrawDebugTrace::Type DrawDebugType = drawDebugType;
	URosGameManager* gameManager = GetRosGameManager();
	if(gameManager != nullptr)
	{
		if(gameManager->GetDrawDebugAllow() == false)
		{
			DrawDebugType = EDrawDebugTrace::None;
		}
	}
	
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 5.0f;
	
	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		this,            // Objet effectuant la trace
		StartLocation,   // Emplacement de départ de la trace
		EndLocation,     // Emplacement de fin de la trace
		UEngineTypes::ConvertToTraceType(TraceChannel),    // Canal de collision à utiliser
		false,           // Trace complexe
		TArray<AActor*>(), // Acteurs à ignorer
		DrawDebugType, // Affichage du débogage
		OutHits,       // Résultat de la trace
		true,             // Ignorer le Self Actor
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	return bHit;
}

bool URosCollisionTraceComponent::SphereTraceMultiByChannel(FVector start, FVector end, float radius,
                                                   ECollisionChannel TraceChannel, TArray<FHitResult>& OutHits)
{
	//UE_LOG(LogTemp, Warning, TEXT("SphereTraceMulti %s"), *GetOwner()->GetName());
	
	FVector StartLocation = start; // L'emplacement de départ de la trace
	FVector EndLocation = end; // L'emplacement de fin de la trace
	float SphereRadius = radius; // Le rayon de la sphère pour la trace
			
	// ECollisionChannel TraceChannel = ECC_Visibility; // Le canal de collision à utiliser
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	// ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(TraceChannel));
			
	// FCollisionQueryParams TraceParams;
	// TraceParams.AddIgnoredActor(this); // Ignorer cet acteur lors de la trace
	
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	URosGameManager* gameManager = GetRosGameManager();
	if(gameManager != nullptr)
	{
		if(gameManager->GetDrawDebugAllow() == false)
		{
			DrawDebugType = EDrawDebugTrace::None;
		}
	}
	
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 1.5f;
	
	bool bHit = UKismetSystemLibrary::SphereTraceMulti
	(
		this,            // Objet effectuant la trace
		StartLocation,   // Emplacement de départ de la trace
		EndLocation,     // Emplacement de fin de la trace
		SphereRadius,    // Rayon de la sphère pour la trace
		UEngineTypes::ConvertToTraceType(TraceChannel),    // Canal de collision à utiliser
		false,           // Trace complexe
		TArray<AActor*>(), // Acteurs à ignorer
		DrawDebugType, // Affichage du débogage
		OutHits,       // Résultat de la trace
		true,             // Ignorer le Self Actor
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	return bHit;
}

bool URosCollisionTraceComponent::SphereTraceMulti(FVector start, FVector end, float radius, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	TArray<FHitResult>& OutHits)
{
	//UE_LOG(LogTemp, Warning, TEXT("SphereTraceMulti %s"), *GetOwner()->GetName());
	
	FVector StartLocation = start; // L'emplacement de départ de la trace
	FVector EndLocation = end; // L'emplacement de fin de la trace
	float SphereRadius = radius; // Le rayon de la sphère pour la trace
			
	// ECollisionChannel TraceChannel = ECC_Visibility; // Le canal de collision à utiliser
	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	// ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(TraceChannel));
			
	// FCollisionQueryParams TraceParams;
	// TraceParams.AddIgnoredActor(this); // Ignorer cet acteur lors de la trace
	
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	URosGameManager* gameManager = GetRosGameManager();
	if(gameManager != nullptr)
	{
		if(gameManager->GetDrawDebugAllow() == false)
		{
			DrawDebugType = EDrawDebugTrace::None;
		}
	}
	
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 1.5f;
	
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		this,            // Objet effectuant la trace
		StartLocation,   // Emplacement de départ de la trace
		EndLocation,     // Emplacement de fin de la trace
		SphereRadius,    // Rayon de la sphère pour la trace
		ObjectTypes,    
		false,           // Trace complexe
		TArray<AActor*>(), // Acteurs à ignorer
		DrawDebugType, // Affichage du débogage
		OutHits,       // Résultat de la trace
		true,             // Ignorer le Self Actor
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	return bHit;
}

