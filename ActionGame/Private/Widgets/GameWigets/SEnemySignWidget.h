// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "ActionGameCharacter.h"
#include "../Styles/UIAssetWidgetStyle.h"

/**
 * 
 */
class SEnemySignWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnemySignWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AActionGameCharacter>, Owner)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )override;

private:
	const struct FUIAssetStyle* UIStyle;

	TWeakObjectPtr<class AActionGameCharacter> Owner;

	TWeakObjectPtr<class AActionGameCharacter> AIEnemy;
};
