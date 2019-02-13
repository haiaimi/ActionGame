// Fill out your copyright notice in the Description page of Project Settings.

#include "UIAssetWidgetStyle.h"

FUIAssetStyle::FUIAssetStyle()
{
}

FUIAssetStyle::~FUIAssetStyle()
{
}

const FName FUIAssetStyle::TypeName(TEXT("FUIAssetStyle"));

const FUIAssetStyle& FUIAssetStyle::GetDefault()
{
	static FUIAssetStyle Default;
	return Default;
}

void FUIAssetStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

