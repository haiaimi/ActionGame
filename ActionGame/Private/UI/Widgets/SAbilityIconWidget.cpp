// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityIconWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"

#define LOCTEXT_NAMESPACE "ActionGame.UI.AbilityIcon"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAbilityIconWidget::Construct(const FArguments& InArgs)
{
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SBox)
			.HeightOverride(100.f)
			.WidthOverride(100.f)
			[
				SNew(SBorder)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.BorderImage(&UIStyle->HeroHeadImages[0])
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Percent","50%"))
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),40))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBox)
			.HeightOverride(100.f)
			.WidthOverride(100.f)
			.RenderTransform(FSlateRenderTransform(FVector2D(0.f,-100.f)))
			[
				SNew(SBorder)
				.BorderImage(BorderBackground)
				.RenderTransformPivot(FVector2D(0.5f, 0.f))
			]
		]
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE