// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Interface/AGWidgetInterface.h"
#include "UI/SBaseMenuWidget.h"
#include "MenuHUD.h"
#include "GameActors/HeroDetailPlatform.h"

/**
 * 
 */
class ACTIONGAME_API SHeroDetailWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SHeroDetailWidget):
		_OwnerHUD(nullptr),
		_PreWidget(nullptr),
		_DetailPlatform(nullptr)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwnerHUD)
	SLATE_ARGUMENT(TSharedPtr<SCompoundWidget>, PreWidget)
	SLATE_ARGUMENT(TWeakObjectPtr<AHeroDetailPlatform>, DetailPlatform)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void BackToPrevious()override;

protected:
	void SetupAnimation();

	void ShowHeroSkinButtons(int32 Index);

private:
	TSharedPtr<SScrollBox> HerosBar;

	TSharedPtr<SScrollBox> HeroSkinsBar;

	TArray<TSharedPtr<SButton>> SkinButtons;

	TArray<TSharedPtr<SButton>> HeroButtons;

	const struct FButtonStyle* ButtonStyle;

	const struct FButtonStyle* ButtonSelectedStyle;

	FCurveSequence AnimSequence;

	FCurveHandle AnimHandle;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;

	TWeakObjectPtr<AHeroDetailPlatform> DetailPlatform;

	FTimerHandle ResetTimer;
};
