// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Player/MenuPlayerController.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuPlayerController>, OwnerController)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	/**…Ë÷√∂Øª≠*/
	void SetupAnimation();

	void HeroDetails();

private:
	FCurveSequence MenuSecquence;

	TArray<FCurveHandle> AnimHandles;

	const struct FButtonStyle* ButtonStyle;

	TWeakObjectPtr<class AMenuPlayerController> OwnerController;
};
