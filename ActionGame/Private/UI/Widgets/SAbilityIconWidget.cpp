// Fill out your copyright notice in the Description page of Project Settings.

#include "SAbilityIconWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"

#define LOCTEXT_NAMESPACE "ActionGame.UI.AbilityIcon"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAbilityIconWidget::Construct(const FArguments& InArgs)
{
	CoolingTime = InArgs._CoolingTime;
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
				.Padding(FMargin(5.f))
				[
					SNew(SBorder)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.VAlign(EVerticalAlignment::VAlign_Center)
					.BorderImage(&UIStyle->HeroHeadImages[0])
					[
						SAssignNew(PercentText, STextBlock)
						.Text(LOCTEXT("Percent","100%"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),23))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
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
				.Padding(FMargin(5.f))
				[
					SNew(SBorder)
					.BorderImage(BorderBackground)
					.RenderTransformPivot(FVector2D(0.5f, 0.f))
					.RenderTransform_Lambda([&]() {
						const float CurLerp = CoolingHandle.GetLerp();
						if (PercentText.IsValid())
						{
							FString Tmp = FString::Printf(TEXT("%3.f"), CurLerp*100.f) + TEXT("%");
							PercentText->SetText(FText::FromString(Tmp));
							
							PercentText->SetVisibility(CurLerp == 1.f ? EVisibility::Hidden : EVisibility::Visible);
						}
						return FSlateRenderTransform(FScale2D(1.f, 1.f - CurLerp));
				})
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