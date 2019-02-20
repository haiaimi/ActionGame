// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "SBaseMenuWidget.h"
#include "SSkinsScrollBox.h"
#include "../Styles/UIAssetWidgetStyle.h"

/**
 * 
 */
class SStartGameWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SStartGameWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void BackToPrevious()override;

private:
	/**ÃÌº””¢–€Õ∑œÒ*/
	void AddHeroHeads();
private:
	TSharedPtr<SScrollBox> HeroList;

	TSharedPtr<SSkinsScrollBox> SkinList;

	const struct FUIAssetStyle* UIStyle;

	const struct FButtonStyle* BackButtonStyle;
};
