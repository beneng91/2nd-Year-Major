// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuantumCollapse : ModuleRules
{
	public QuantumCollapse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "CableComponent" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "CableComponent" });
    }
}
