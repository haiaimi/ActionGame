// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "SHeroShowItem.h"

/**
 *  ∆§∑Ù—°‘ÒΩÁ√Ê
 */
class SSkinsScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSkinsScrollBox):
		_SkinImageBrushs(nullptr)
	{}
	SLATE_ARGUMENT(const TArray<struct FSlateBrush>*, SkinImageBrushs)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetSkinImages(TArray<struct FSlateBrush>* InImages);

	void SetSkinItems();

private:
	TSharedPtr<SScrollBox> SkinContainer;

	TArray<TSharedPtr<SHeroShowItem>> SkinItems;

	const TArray<struct FSlateBrush>* SkinImageBrushs;
};
