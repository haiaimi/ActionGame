// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGame.h"
#include "Modules/ModuleManager.h"
#include "UI/Styles/FActionGameStyle.h"

class FActionGameModule :public FDefaultGameModuleImpl
{
	virtual void StartupModule()override
	{
		//加载样式
		FSlateStyleRegistry::UnRegisterSlateStyle(FActionGameStyle::GetStyleSetName());
		FActionGameStyle::Initialize();
	}

	virtual void ShutdownModule()override
	{
		//卸载样式
		FActionGameStyle::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FActionGameModule, ActionGame, "ActionGame" );
 

DEFINE_LOG_CATEGORY(LogHM)