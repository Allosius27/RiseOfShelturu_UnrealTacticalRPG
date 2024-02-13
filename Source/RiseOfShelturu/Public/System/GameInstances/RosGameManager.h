// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "RosGameManager.generated.h"

class ARosPlayerController;
class UObject;

DECLARE_LOG_CATEGORY_EXTERN(LogRos, Log, All);

/**
 * 
 */
UCLASS(Config = Game)
class RISEOFSHELTURU_API URosGameManager : public UGameInstance
{
	GENERATED_BODY()

public:

	URosGameManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region PROPERTIES

public:

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter=GetDrawDebugAllow, Category = "GameManager|Debug")
	bool DrawDebugAllow = false;

private:

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma region GETTERS

		ARosPlayerController* GetPrimaryPlayerController() const;

		UFUNCTION(BlueprintPure, BlueprintCallable)
		bool GetDrawDebugAllow() const { return DrawDebugAllow; }

	#pragma endregion
	
	UFUNCTION(BlueprintCallable)
	void SetDrawDebugAllow(bool value);
	
protected:

#pragma endregion
};
