// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UtilitySystem : ModuleRules
{
	public UtilitySystem(ReadOnlyTargetRules Target) : base(Target)
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
	        "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput"
        });
        
        PrivateDependencyModuleNames.AddRange(
	        new string[]
	        {
		        // Default Modules
		        "Core",
		        "CoreUObject",
		        "Engine",
		        "Slate",
		        "SlateCore",
				
		        //New Modules
		        "Json",
		        "JsonUtilities",
		        "LevelSequence",
		        "Niagara",
		        "AssetRegistry",
		        "MovieScene",
		        "MovieSceneTracks",
		        "AIModule",
				
		        // New Modules - Editor Only
		        // "AssetTools",
		        // "UnrealEd",
		        // "MaterialEditor",
		        // "KismetCompiler",
		        // "SequencerScriptingEditor",
		        // "MovieSceneTools",
		        // "MovieSceneCapture",
		        // "AVIWriter",
		        // "LevelEditor"
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
