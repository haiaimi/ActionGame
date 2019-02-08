// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "Player/MenuPlayerController.h"
#include "UI/MenuHUD.h"
#include "UI/SBaseMenuWidget.h"

/**
 * 
 */
class SMainMenuWidget : public SBaseMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget):
		_OwnerHUD(nullptr),
		_OwnerController(nullptr)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwnerHUD)
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuPlayerController>, OwnerController)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void ToMainMenu();

private:
	/**…Ë÷√∂Øª≠*/
	void SetupAnimation();

	void HeroDetails();

private:
	TSharedPtr<SOverlay> MenuOverlay;

	TSharedPtr<SVerticalBox> MenuContainer;

	FCurveSequence MenuSequence;

	TArray<FCurveHandle> AnimHandles;

	const struct FButtonStyle* ButtonStyle;

	TWeakObjectPtr<class AMenuPlayerController> OwnerController;

	TWeakObjectPtr<class AMenuHUD> OwnerHUD;

	AActor* MenuShowActor;
};
