// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGame.h"
#include "Modules/ModuleManager.h"
#include "UI/Styles/FActionGameStyle.h"

class FActionGameModule :public FDefaultGameModuleImpl
{
	virtual void StartupModule()override
	{
		//������ʽ
		FSlateStyleRegistry::UnRegisterSlateStyle(FActionGameStyle::GetStyleSetName());
		FActionGameStyle::Initialize();
	}

	virtual void ShutdownModule()override
	{
		//ж����ʽ
		FActionGameStyle::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FActionGameModule, ActionGame, "ActionGame" );
 

DEFINE_LOG_CATEGORY(LogHM)