// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "SBaseMenuWidget.h"
#include "MenuHUD.h"

/**
 * 
 */
class SSettingsWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SSettingsWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwnerHUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void SetupAnimation();

	virtual void BackToPrevious()override;

	virtual void BackToShow()override;

	FORCEINLINE float GetCurAnimLerp() { return AnimHandle.GetLerp(); }

private:
	const struct FButtonStyle* ButtonStyle;

	const struct FButtonStyle* BackButtonStyle;

	const struct FButtonStyle* TagButtonStyle;

	FCurveSequence AnimSequence;

	FCurveHandle AnimHandle;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;
};
