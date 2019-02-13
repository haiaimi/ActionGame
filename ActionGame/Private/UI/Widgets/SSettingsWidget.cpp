// Fill out your copyright notice in the Description page of Project Settings.

#include "SSettingsWidget.h"
#include "SlateOptMacros.h"
#include "SBackgroundBlur.h"
#include "../Styles/FActionGameStyle.h"
#include "SSelectBoxWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSettingsWidget::Construct(const FArguments& InArgs)
{
	using FPaddingParam = TAttribute<FMargin>;
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.2f));

	FPaddingParam ::FGetter TransformGetter;
	TransformGetter.BindLambda([this]() {
		const float CurLerp = AnimHandle.GetLerp();
		SetRenderOpacity(CurLerp);
		float Bottom = 400.f - 400.f*CurLerp;
		return FMargin(1980.f - 1980.f*CurLerp, 0.f, -1980.f + 1980.f*CurLerp, 0.f);
		});
	FPaddingParam  TransformParam;
	TransformParam.Bind(TransformGetter);

	TArray<FString> TempText = { TEXT("Low"),TEXT("Medium"),TEXT("High"),TEXT("Ultra") };
	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	.Padding(TransformParam)
	[
		SNew(SBackgroundBlur)
		.BlurStrength(7.f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("设置")))
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),53))
			]
			+SVerticalBox::Slot()
			.FillHeight(10.f)
			[
				SNew(SBorder)
				.BorderImage(BorderBackground)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(true)
						+SScrollBox::Slot()
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("所有设置")))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.f,0.f,0.f,1.f)))
						]
						+SScrollBox::Slot()
						[
							SNew(SButton)
							.ButtonStyle(ButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("画面")))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
							]
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(3.f)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(true)
						+SScrollBox::Slot()
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("画面设置")))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.f,0.f,0.f,1.f)))
						]
						+SScrollBox::Slot()
						[
							SNew(SSelectBoxWidget)
							.SelectName(FText::FromString(TEXT("抗锯齿")))
							.SelectContent(TempText)
							.CurSelection(2.f)
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(2.f)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
							SNew(STextBlock)
							.AutoWrapText(true)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							.Text(FText::FromString(TEXT("content content content content content content content content ...")))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),15))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.f,0.f,0.f,1.f)))
						
					]
				]
			]
			+SVerticalBox::Slot()
			.FillHeight(1.5f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.f)
				[
					SNew(SButton)
					.ButtonStyle(BackButtonStyle)
				]
				+SHorizontalBox::Slot()
				.FillWidth(15.f)
				[
					SNew(SBorder)
				]
			]
		]
	];

	SetupAnimation();
}

void SSettingsWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
	AnimSequence.Play(this->AsShared());
}

void SSettingsWidget::BackToPrevious()
{
}

void SSettingsWidget::BackToShow()
{
	AnimSequence.Reverse();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
