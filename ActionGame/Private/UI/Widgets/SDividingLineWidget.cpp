// Fill out your copyright notice in the Description page of Project Settings.

#include "SDividingLineWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"
#include "../Styles/UIAssetWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SDividingLineWidget::Construct(const FArguments& InArgs)
{
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SImage)
			.Image(&UIStyle->DividingLine)
			.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.7f)))
			.RenderTransform(FSlateRenderTransform(FScale2D(1.7f, 0.7f)))
			.RenderTransformPivot(FVector2D(0.5f,0.5f))
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(InArgs._CategoryName)
			.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),13))
			.ColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f)))
		]
		+SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SNew(SImage)
			.Image(&UIStyle->DividingLine)
			.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.7f)))
			.RenderTransform(FSlateRenderTransform(FScale2D(1.7f, 0.7f)))
			.RenderTransformPivot(FVector2D(0.5f,0.5f))
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
