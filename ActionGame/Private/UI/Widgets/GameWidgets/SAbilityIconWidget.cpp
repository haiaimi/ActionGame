﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityIconWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"
#include "SBackgroundBlur.h"

#define LOCTEXT_NAMESPACE "ActionGame.UI.AbilityIcon"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAbilityIconWidget::Construct(const FArguments& InArgs)
{
	Owner = InArgs._Owner;
	CoolingTime = InArgs._CoolingTime;
	EAbilityType::Type AbilityType = InArgs._AbilityType;
	BorderBackground = MakeShareable(new FSlateBrush());
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(100.f)
			.WidthOverride(100.f)
			[
				SNew(SBorder)
				.Padding(FMargin(5.f))
				[
					SNew(SBorder)
					.BorderImage(BorderBackground.Get())
					.RenderTransformPivot(FVector2D(0.5f, 0.f))
					.RenderTransform_Lambda([this, AbilityType]() {
						const float CoolingRate = Owner->GetCoolingRate(AbilityType);
						if (PercentText.IsValid() && Owner.IsValid())
						{
							FString Tmp = FString::Printf(TEXT("%3.f"), CoolingRate*100.f) + TEXT("%");
							PercentText->SetText(FText::FromString(Tmp));
							PercentText->SetVisibility(CoolingRate == 1.f ? EVisibility::Hidden : EVisibility::Visible);
						}
						return FSlateRenderTransform(FScale2D(1.f, 1.f - CoolingRate));
					})
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(100.f)
			.WidthOverride(100.f)
			.RenderTransform(FSlateRenderTransform(FVector2D(0.f,-100.f)))
			[
				SNew(SBorder)
				.Padding(FMargin(5.f))
				[
					SNew(SBackgroundBlur)
					.BlurStrength(7.f)
					[
						SNew(SBorder)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						.VAlign(EVerticalAlignment::VAlign_Bottom)
						.BorderImage(&UIStyle->AbilityIcons[InArgs._PlayerIndex].Images[(int32)AbilityType - 1])
						[
							SAssignNew(PercentText, STextBlock)
							.Text(LOCTEXT("Percent","100%"))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),15))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							.RenderTransform(FSlateRenderTransform(FVector2D(-5.f, 5.f)))
						]
					]
				]
			]
		]
	];
	SetupAnimation();
	AnimSequence.Play(this->AsShared());
}

void SAbilityIconWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();

	CoolingHandle = AnimSequence.AddCurve(0.f, CoolingTime);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE