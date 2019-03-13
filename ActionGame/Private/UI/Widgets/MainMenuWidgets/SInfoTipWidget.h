// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SInfoTipWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInfoTipWidget)
	{}
	SLATE_ARGUMENT(FText, TipContent)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetupAnimation();

private:
	FCurveSequence AnimSequence;

	TSharedPtr<FSlateBrush> BorderBackground;

	FCurveHandle AnimHandle;
};
