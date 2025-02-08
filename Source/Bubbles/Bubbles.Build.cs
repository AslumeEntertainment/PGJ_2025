// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bubbles : ModuleRules
{
	public Bubbles(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        PublicDependencyModuleNames.AddRange(
            new string[] 
            { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "InputCore", 
                "EnhancedInput",
                "OnlineSubsystem",
                "OnlineSubsystemUtils" 
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "UMG",
                "OnlineSubsystem",
                "OnlineSubsystemUtils"
				// ... add private dependencies that you statically link with here ...	
			}
            );

    }
}
