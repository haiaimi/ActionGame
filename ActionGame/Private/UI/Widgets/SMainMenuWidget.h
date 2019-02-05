// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	/**…Ë÷√∂Øª≠*/
	void SetupAnimation();

private:
	FCurveSequence MenuSecquence;

	TArray<FCurveHandle> AnimHandles;

	const struct FButtonStyle* ButtonStyle;
};
