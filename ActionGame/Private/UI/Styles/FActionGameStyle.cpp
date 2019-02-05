// Fill out your copyright notice in the Description page of Project Settings.

#include "FActionGameStyle.h"
#include "SlateGameResources.h"

TSharedPtr< class FSlateStyleSet > FActionGameStyle::ActionGameStyleInstance = NULL;

void FActionGameStyle::Initialize()
{
	//≥ı ºªØSlateStyleSet
	if (!ActionGameStyleInstance.IsValid())
	{
		ActionGameStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*ActionGameStyleInstance);
	}
}

void FActionGameStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*ActionGameStyleInstance);
	ensure(ActionGameStyleInstance.IsUnique());
	ActionGameStyleInstance.Reset();
}

const ISlateStyle & FActionGameStyle::Get()
{
	return *ActionGameStyleInstance;
}

FName FActionGameStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ActionGameStyle"));
	return StyleSetName;
}

TSharedRef<class FSlateStyleSet> FActionGameStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FActionGameStyle::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	FSlateStyleSet& Style = StyleRef.Get();

	return StyleRef;
}
