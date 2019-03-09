// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "SBaseMenuWidget.h"
#include "SSkinsScrollBox.h"
#include "../Styles/UIAssetWidgetStyle.h"
#include "MenuHUD.h"

/**
 * 
 */
class SStartGameWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SStartGameWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwnerHUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void BackToPrevious()override;

	virtual void BackToShow()override;

	FORCEINLINE float GetCurAnimLerp() { return StartingPageHandle.GetLerp(); }

private:
	void SetupAnimation();

	/**ÃÌº””¢–€Õ∑œÒ*/
	void AddHeroHeads();

	void StartGame();

private:
	FCurveSequence AnimSequence;

	FCurveHandle StartingPageHandle;
		
	TArray<FCurveHandle> AnimHandles;

	TSharedPtr<SScrollBox> HeroList;

	TSharedPtr<STextBlock> HeroName;

	TSharedPtr<SSkinsScrollBox> SkinList;

	const struct FUIAssetStyle* UIStyle;

	const struct FButtonStyle* BackButtonStyle;

	const struct FButtonStyle* ButtonStyle;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;

	int32 CurSelectIndex;
};
