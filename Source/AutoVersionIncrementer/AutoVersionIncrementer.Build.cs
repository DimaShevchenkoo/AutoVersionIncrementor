// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutoVersionIncrementer : ModuleRules
{
	public AutoVersionIncrementer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"Blutility",
				"UMGEditor",
				"LevelEditor",
				"ToolMenus",
				"UMG",
				"InputCore", 
				"EditorScriptingUtilities",
			});
	}
}
