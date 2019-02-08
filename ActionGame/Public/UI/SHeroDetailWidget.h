// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Interface/AGWidgetInterface.h"
#include "UI/SBaseMenuWidget.h"
#include "MenuHUD.h"

/**
 * 
 */
class ACTIONGAME_API SHeroDetailWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SHeroDetailWidget):
		_OwnerHUD(nullptr),
		_PreWidget(nullptr)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwnerHUD)
	SLATE_ARGUMENT(TSharedPtr<SCompoundWidget>, PreWidget)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void BackToPrevious()override;

protected:
	void SetupAnimation();

private:
	const struct FButtonStyle* ButtonStyle;

	FCurveSequence AnimSequence;

	FCurveHandle AnimHandle;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;

	FTimerHandle ResetTimer;
};
