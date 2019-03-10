// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "../Styles/UIAssetWidgetStyle.h"
#include "AGPlayerController.h"

/**
 * 
 */
class SPauseMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPauseMenuWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AAGPlayerController>, OwnerController)
	SLATE_ARGUMENT(FSimpleDelegate, BackDelegate)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetContinueButtonEnable(bool bEnabled);

private:
	void ContinueGame();

	void QuitGame();

private:
	const struct FUIAssetStyle* UIStyle;

	TSharedPtr<SButton> ContinueButton;

	const struct FButtonStyle* ButtonStyle;

	FSlateFontInfo FontInfo;

	TWeakObjectPtr<class AAGPlayerController> OwnerController;
};
