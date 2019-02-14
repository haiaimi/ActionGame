// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 *   UI中的分割线
 */
class SDividingLineWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDividingLineWidget)
	{}
	SLATE_ARGUMENT(FText, CategoryName)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	const struct FUIAssetStyle* UIStyle;
};
