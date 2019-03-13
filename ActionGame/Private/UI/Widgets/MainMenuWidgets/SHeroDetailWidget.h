// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Interface/AGWidgetInterface.h"
#include "SBaseMenuWidget.h"
#include "GameActors/HeroDetailPlatform.h"
#include "UI/MenuHUD.h"
#include "SInfoTipWidget.h"

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

	virtual void BackToShow()override;

	FORCEINLINE float GetCurAnimLerp() { return AnimHandle.GetLerp(); }

protected:
	void SetupAnimation();

	void ShowHeroSkinButtons(int32 Index);

	void ShowTips(const FText& ShowText);

	void CloseWidget();

private:
	TSharedPtr<SOverlay> DetailOverlay;

	TSharedPtr<SScrollBox> HerosBar;

	TSharedPtr<SScrollBox> HeroSkinsBar;

	TSharedPtr<SHorizontalBox> AbilityButtonContainer;

	TArray<TSharedPtr<SButton>> SkinButtons;

	TArray<TSharedPtr<SButton>> HeroButtons;

	TArray<TSharedPtr<SButton>> AbilityButtons;

	TSharedPtr<SBox> TipWidget;

	const struct FButtonStyle* ButtonStyle;

	const struct FButtonStyle* ButtonSelectedStyle;

	const struct FButtonStyle* BackButtonStyle;

	FCurveSequence AnimSequence;

	FCurveHandle AnimHandle;

	FCurveSequence SkinButtonSequence;

	TArray<FCurveHandle> SkinButtonHandles;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;

	TWeakObjectPtr<AHeroDetailPlatform> DetailPlatform;

	FTimerHandle ResetTimer;

	TSharedPtr<FSlateBrush> BorderBackground;
};
