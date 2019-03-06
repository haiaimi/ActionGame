// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "../Styles/UIAssetWidgetStyle.h"

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
	SLATE_ARGUMENT(float, CoolingTime)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	void SetupAnimation();

private:
	const struct FUIAssetStyle* UIStyle;

	float CoolingTime;

	FCurveSequence AnimSequence;

	FCurveHandle CoolingHandle;

	TSharedPtr<STextBlock> PercentText;
};
