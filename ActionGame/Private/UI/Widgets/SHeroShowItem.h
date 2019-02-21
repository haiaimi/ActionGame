// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "../Styles/UIAssetWidgetStyle.h"

/**
 * 
 */
class SHeroShowItem : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHeroShowItem):
		_BorderScale(1.f,1.f),
		_ImageScale(0.8f,0.8f),
		_HoverScale(1.f),
		_Image(nullptr)
	{}
	SLATE_ARGUMENT(FVector2D, BorderScale)
	SLATE_ARGUMENT(FVector2D, ImageScale)
	SLATE_ARGUMENT(float, HoverScale)
	SLATE_ARGUMENT(const FSlateBrush*, Image)
	SLATE_EVENT(FSimpleDelegate, OnPressed)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetImage(const FSlateBrush* InBrush);

private:
	TSharedPtr<SBorder> ItemBorder;

	TSharedPtr<SImage> ItemImage;

	const struct FUIAssetStyle* UIStyle;

	FVector2D BorderScale;

	FVector2D ImageScale;

	float HoverScale;

	const FSlateBrush* Image;
	
	const FButtonStyle* TagButtonStyle;
};
