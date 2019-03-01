// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionGameLoadingScreen : ModuleRules
{
	public ActionGameLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "MoviePlayer", "InputCore" });

        PrivateIncludePaths.Add("../../ActionGame/Source/ActionGameLoadingScreen/Private");

        PrivateIncludePaths.AddRange(
           new string[] {
               // "ActionGame/Private/UI/Widgets"
           });
    }
}
