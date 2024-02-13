// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityCustomTimer.h"

void UUtilityCustomTimer::Update(float deltaTime)
{
	if(TimerIsRunning && TimerIsInPause == false)
	{
		if(Timer < TimerDuration)
		{
			Timer += deltaTime;
		}
		else
		{
			Timer = 0.0f;
			TimerIsRunning = false;

			DispatcherOnEndTimer.Broadcast();
		}
	}
}

void UUtilityCustomTimer::SetActiveTimer(bool value, float duration, bool startIsInPause)
{
	Timer = 0.0f;
	TimerDuration = duration;
	TimerIsInPause = startIsInPause;
	TimerIsRunning = value;

	if(TimerIsRunning)
	{
		DispatcherOnStartTimer.Broadcast();
	}
}
