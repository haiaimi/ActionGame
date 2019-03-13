// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "../Styles/UIAssetWidgetStyle.h"
#include "ActionGameType.h"
#include "ActionGameCharacter.h"

/**
 * 
 */
using FRenderTransformParam = TAttribute<TOptional<FSlateRenderTransform>>;

class SAbilityIconWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAbilityIconWidget):
		_CoolingTime(1.f)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AActionGameCharacter>, Owner)
	SLATE_ARGUMENT(float, CoolingTime)
	SLATE_ARGUMENT(EAbilityType::Type, AbilityType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	void SetupAnimation();

	const struct FUIAssetStyle* UIStyle;

	TSharedPtr<FSlateBrush> BorderBackground;

	float CoolingTime;

	FCurveSequence AnimSequence;

	FCurveHandle CoolingHandle;

	TSharedPtr<STextBlock> PercentText;

	TWeakObjectPtr<AActionGameCharacter> Owner;
};
