// Fill out your copyright notice in the Description page of Project Settings.

#include "SHeroShowItem.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHeroShowItem::Construct(const FArguments& InArgs)
{
	OnHovered = InArgs._OnHovered;
	BorderScale = InArgs._BorderScale;
	ImageScale = InArgs._ImageScale;
	HoverScale = InArgs._HoverScale;
	Image = InArgs._Image;

	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	TagButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("TagButtonStyle"));

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SButton)
		.ButtonStyle(TagButtonStyle)
		.OnHovered_Lambda([&]() {
		ItemBorder->SetRenderTransform(FSlateRenderTransform(FScale2D(BorderScale*HoverScale)));
		OnHovered.ExecuteIfBound();
			})
		.OnUnhovered_Lambda([&]() {
		ItemBorder->SetRenderTransform(FSlateRenderTransform(FScale2D(BorderScale)));
			})
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		.OnPressed(InArgs._OnPressed)
		[
			SAssignNew(ItemBorder, SBorder)
			.RenderTransform(FSlateRenderTransform(FScale2D(BorderScale)))
			.RenderTransformPivot(FVector2D(0.5f,0.5f))
			.BorderImage(&UIStyle->HeroItemBorder)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(ItemImage, SImage)
				.Image(Image)
				.RenderTransform(FSlateRenderTransform(FScale2D(ImageScale)))
				.RenderTransformPivot(FVector2D(0.5f,0.5f))
			]
		]
	];
}

void SHeroShowItem::SetImage(const FSlateBrush* InBrush)
{
	ItemImage->SetImage(InBrush);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
