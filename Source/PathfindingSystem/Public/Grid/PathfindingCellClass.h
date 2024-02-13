// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PATHFINDINGSYSTEM_API PathfindingCellClass
{
public:
	PathfindingCellClass();
	
	FVector2D pos;
	FVector worldPos;
	BYTE cost;
	USHORT bestCost;
	bool checked;
	int index;
	FVector dir;
	FVector normal;
	
	~PathfindingCellClass();
};
