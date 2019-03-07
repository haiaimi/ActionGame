// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "ActionGameCharacter.h"

/**
 * HUD¿Ø¼þ
 */
class SHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHUDWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AActionGameCharacter>, Owner)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )override;

private:
	void SetupAnimation();

private:
	TSharedPtr<SHorizontalBox> AbilityContainer;

	FCurveSequence AnimSequence;

	FCurveHandle AbilityIconHandle;

	TWeakObjectPtr<AActionGameCharacter> Owner;

	FVector2D AbilityTransform;
};
