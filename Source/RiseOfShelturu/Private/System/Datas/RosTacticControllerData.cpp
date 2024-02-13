// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Datas/RosTacticControllerData.h"

URosTacticControllerData::URosTacticControllerData(const FObjectInitializer& ObjectInitializer)
{
}

float URosTacticControllerData::GetLocationInterpolation() const
{
	return LocationInterpolation;
}

float URosTacticControllerData::GetLocationSpeed() const
{
	return LocationSpeed;
}

float URosTacticControllerData::GetRotationInterpolation() const
{
	return RotationInterpolation;
}

float URosTacticControllerData::GetRotationSpeed() const
{
	return RotationSpeed;
}

float URosTacticControllerData::GetZoomInterpolation() const
{
	return ZoomInterpolation;
}

float URosTacticControllerData::GetZoomSpeed() const
{
	return ZoomSpeed;
}

float URosTacticControllerData::GetZoomMin() const
{
	return ZoomMin;
}

float URosTacticControllerData::GetZoomMax() const
{
	return ZoomMax;
}
