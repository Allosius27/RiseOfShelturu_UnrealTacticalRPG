// Fill out your copyright notice in the Description page of Project Settings.


#include "Development/RosDeveloperSettings.h"
#include "Misc/App.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RosDeveloperSettings)

#define LOCTEXT_NAMESPACE "RosCheats"

URosDeveloperSettings::URosDeveloperSettings()
{
}

FName URosDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

#if WITH_EDITOR
void URosDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ApplySettings();
}

void URosDeveloperSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	ApplySettings();
}

void URosDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();

	ApplySettings();
}

void URosDeveloperSettings::ApplySettings()
{
}

void URosDeveloperSettings::OnPlayInEditorStarted() const
{
}

#endif

#undef LOCTEXT_NAMESPACE