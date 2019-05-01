// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionGame : ModuleRules
{
	public ActionGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core",
                                                            "CoreUObject",
                                                            "Engine",
                                                            "InputCore",
                                                            "HeadMountedDisplay",
                                                            "Json",
                                                            "GameplayTasks",
                                                            "AIModule",
                                                            "NavigationSystem",
                                                            "LevelSequence",
                                                            "CinematicCamera",
                                                            "MovieScene" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(
           new string[] {
                "ActionGame/Private/UI/Widgets",
                "ActionGame/Private/UI/Widgets/GameWidgets",
                "ActionGame/Private/UI/Widgets/MainMenuWidgets",
                "ActionGame/Private/UI/Styles"
           });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "ActionGameLoadingScreen"
            });
    }
}
