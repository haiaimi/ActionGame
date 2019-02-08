// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHeroDetailWidget.h"
#include "SlateOptMacros.h"
#include "Styles/FActionGameStyle.h"
#include "Widgets/SMainMenuWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHeroDetailWidget::Construct(const FArguments& InArgs)
{
	using FPaddingParam = TAttribute<FMargin>;

	PreWidget = InArgs._PreWidget;
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.1f));
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	FPaddingParam::FGetter PadingGetter;
	PadingGetter.BindLambda([&]() {
		const float CurLerp = AnimHandle.GetLerp();
		return FMargin(1970.f - 1920.f*CurLerp, 40.f, 0.f, 0.f);
		});
	FPaddingParam PaddingParam;
	PaddingParam.Bind(PadingGetter);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(PaddingParam)
		[
			SNew(SBox)
			.HeightOverride(900)
			.WidthOverride(1800)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1)
				[
					SNew(SBorder)
					.BorderImage(BorderBackground)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(true)
						+SScrollBox::Slot()
						[
							SNew(SBorder)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.Padding(0.f)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("英雄"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
						+SScrollBox::Slot()
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("Aurora"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
					
				]
				+SHorizontalBox::Slot()
				.FillWidth(1)
				[
					SNew(SBorder)
					.BorderImage(BorderBackground)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(true)
						+SScrollBox::Slot()
						[
							SNew(SBorder)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.Padding(0.f)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("皮肤"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
						+SScrollBox::Slot()
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("标准"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
				+SHorizontalBox::Slot()
				.FillWidth(4)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.FillHeight(4)
					[
						SNew(SBox)
					]
					+SVerticalBox::Slot()
					.FillHeight(1)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(1.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("英雄概况"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
						+SHorizontalBox::Slot()
						.FillWidth(1.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("A"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
						+SHorizontalBox::Slot()
						.FillWidth(1.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("B"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
						+SHorizontalBox::Slot()
						.FillWidth(1.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("C"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
			]
			
		]
	];

	SetupAnimation();
}

void SHeroDetailWidget::BackToPrevious()
{
	AnimSequence.Reverse();
	
	if(PreWidget.IsValid())
	{
		SMainMenuWidget* ptr = static_cast<SMainMenuWidget*>(PreWidget.Get());
		ptr->ToMainMenu();
		SetEnabled(false);
	}
}

void SHeroDetailWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::QuadOut);

	AnimSequence.Play(this->AsShared());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION