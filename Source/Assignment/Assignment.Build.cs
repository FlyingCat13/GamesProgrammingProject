// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assignment : ModuleRules
{
	public Assignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "LevelSequence", "MovieScene", "Niagara", "NavigationSystem", "GameplayTasks" });

        PublicIncludePaths.AddRange(new string[] {
            "Assignment",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Public",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Classes",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Private"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
