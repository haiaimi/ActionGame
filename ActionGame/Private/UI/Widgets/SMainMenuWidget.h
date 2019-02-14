// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Player/MenuPlayerController.h"
#include "UI/MenuHUD.h"
#include "SHeroDetailWidget.h"
#include "SBaseMenuWidget.h"
#include "SSettingsWidget.h"

/**
 * 
 */
class SMainMenuWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget):
		_OwnerHUD(nullptr),
		_OwnerController(nullptr)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwnerHUD)
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuPlayerController>, OwnerController)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void BackToShow()override;

	TSharedPtr<SOverlay>& GetMenuOverlay();

private:
	/**…Ë÷√∂Øª≠*/
	void SetupAnimation();

	void HeroDetails();

	void GameSetting();

	void QuitGame();

private:
	TSharedPtr<SOverlay> MenuOverlay;

	TSharedPtr<SVerticalBox> MenuContainer;

	TSharedPtr<class SHeroDetailWidget> HeroDetail;

	TArray<TSharedPtr<class SBorder>> ButtonBorders;

	TSharedPtr<class SBox> SettingsBox;

	TSharedPtr<class SSettingsWidget> SettingWidget;

	FCurveSequence MenuSequence;

	TArray<FCurveHandle> AnimHandles;

	const struct FButtonStyle* ButtonStyle;

	TWeakObjectPtr<class AMenuPlayerController> OwnerController;

	TWeakObjectPtr<class AMenuHUD> OwnerHUD;

	AActor* MenuShowActor;
};
