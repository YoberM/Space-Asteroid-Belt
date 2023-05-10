// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UFOCPP : ModuleRules
{
	public UFOCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
