// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RosDevelopmentTypes.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "RosDeveloperSettings.generated.h"

struct FPropertyChangedEvent;

/**
 * Developer settings / editor cheats
 */
UCLASS(config=EditorPerProjectUserSettings)
class RISEOFSHELTURU_API URosDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	URosDeveloperSettings();

#pragma region PROPERTIES

public:

	// Do the full game flow when playing in the editor, or skip 'waiting for player' / etc... game phases?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, config, Category="DeveloperSettings")
	bool bTestFullGameFlowInPIE = false;

	// List of cheats to auto-run during 'play in editor'
	UPROPERTY(config, EditAnywhere, Category="DeveloperSettings")
	TArray<FRosCheatToRun> CheatsToRun;

#if WITH_EDITORONLY_DATA
	/** A list of common maps that will be accessible via the editor detoolbar */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category="DeveloperSettings|Maps", meta=(AllowedClasses="/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonEditorMaps;
#endif

#pragma endregion

#pragma region EVENTS

#pragma endregion

#pragma region FUNCTIONS

public:

	#pragma  region GETTERS

		//~UDeveloperSettings interface
		virtual FName GetCategoryName() const override;
		//~End of UDeveloperSettings interface

	#pragma endregion 

#if WITH_EDITOR

	// Called by the editor engine to let us pop reminder notifications when cheats are active
	void OnPlayInEditorStarted() const;

	//~UObject interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;
	virtual void PostInitProperties() override;
	//~End of UObject interface

#endif
	
protected:

#if WITH_EDITOR
	
private:
	void ApplySettings();

#endif

#pragma endregion
};
