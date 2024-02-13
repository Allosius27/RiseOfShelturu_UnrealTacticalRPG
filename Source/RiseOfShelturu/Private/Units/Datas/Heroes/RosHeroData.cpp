// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Datas/Heroes/RosHeroData.h"

URosHeroData::URosHeroData(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InputConfig = nullptr;
}

float URosHeroData::GetZoomInterpolation() const
{
	return ZoomInterpolation;
}

float URosHeroData::GetZoomSpeed() const
{
	return ZoomSpeed;
}

float URosHeroData::GetZoomMin() const
{
	return ZoomMin;
}

float URosHeroData::GetZoomMax() const
{
	return ZoomMax;
}
