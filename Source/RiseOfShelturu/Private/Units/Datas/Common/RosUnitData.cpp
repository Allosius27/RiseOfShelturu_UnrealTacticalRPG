// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Datas/Common/RosUnitData.h"

URosUnitData::URosUnitData(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PawnClass = nullptr;
}

float URosUnitData::GetHealth() const
{
	return Health;
}

float URosUnitData::GetDamages() const
{
	return Damages;
}

float URosUnitData::GetHealing() const
{
	return Healing;
}

int URosUnitData::GetMovementPoints() const
{
	return MovementPoints;
}

int URosUnitData::GetActionPoints() const
{
	return ActionPoints;
}

int URosUnitData::GetInitiative() const
{
	return Initiative;
}


float URosUnitData::GetWorldSpeed() const
{
	return WorldSpeed;
}

float URosUnitData::GetTacticSpeed() const
{
	return TacticSpeed;
}

ERosBattleTeam URosUnitData::GetBattleTeam() const
{
	return BattleTeam;
}
