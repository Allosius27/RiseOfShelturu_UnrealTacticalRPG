// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RiseOfShelturuTarget : TargetRules
{
	public RiseOfShelturuTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("RiseOfShelturu");
        ExtraModuleNames.Add("PathfindingSystem");
        ExtraModuleNames.Add("UtilitySystem");
    }
}
