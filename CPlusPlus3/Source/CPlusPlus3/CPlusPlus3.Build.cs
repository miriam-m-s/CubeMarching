// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPlusPlus3 : ModuleRules
{
	public CPlusPlus3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent" });
	}
}
