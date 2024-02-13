// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RiseOfShelturu : ModuleRules
{
	public RiseOfShelturu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
		
        PublicDependencyModuleNames.AddRange(new string[]
        {
	        "Core", 
	        "CoreUObject", 
	        "Engine", 
	        "InputCore", 
	        "HeadMountedDisplay", 
	        "NavigationSystem", 
	        "AIModule", 
	        "Niagara", 
	        "EnhancedInput",
	        "PhysicsCore",
	        "NPC_Optimizator",
	        "PathfindingSystem",
	        "UtilitySystem",
	        "GameplayAbilities"
        });
        
        PrivateDependencyModuleNames.AddRange(
	        new string[]
	        {
		        "CoreUObject",
		        "Engine",
		        "Slate",
		        "SlateCore",
		        "GameplayAbilities", 
		        "GameplayTags", 
		        "GameplayTasks",
		        "CommonInput",
		        // ... add private dependencies that you statically link with here ...	
	        }
        );
		
		
        DynamicallyLoadedModuleNames.AddRange(
	        new string[]
	        {
		        // ... add any modules that your module loads dynamically here ...
	        }
        );
    }
}
