// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UtilityCustomTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartTimerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTimerSignature);

/**
 * 
 */
UCLASS()
class UTILITYSYSTEM_API UUtilityCustomTimer : public UObject
{
	GENERATED_BODY()

#pragma region PUBLIC
	
public:

#pragma region UPROPERTIES

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetTimerIsRunning() const { return TimerIsRunning; }
	
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetTimer() const { return Timer; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetTimerDuration() const { return TimerDuration; }


	UPROPERTY(BlueprintAssignable)
	FStartTimerSignature DispatcherOnStartTimer;
	
	UPROPERTY(BlueprintAssignable)
	FEndTimerSignature DispatcherOnEndTimer;

#pragma endregion

#pragma region UFUNCTIONS

	UFUNCTION(BlueprintCallable)
	void Update(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void SetActiveTimer(bool value, float duration, bool startIsInPause = false);

#pragma endregion 

#pragma endregion

#pragma region PROTECTED

protected:

#pragma region UPROPERTIES

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetTimerIsRunning, Category= "Protected")
	bool TimerIsRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Protected")
	bool TimerIsInPause = false;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetTimer, Category= "Protected")
	float Timer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetTimerDuration, Category= "Protected")
	float TimerDuration = 10.0f;

#pragma endregion 

#pragma region METHODS
		
		

#pragma endregion 

#pragma endregion 
};
