// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ACTIONGAME_API SHeroDetailWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHeroDetailWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	const struct FButtonStyle* ButtonStyle;
};
