﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHeroDetailWidget.h"
#include "SlateOptMacros.h"
#include "Styles/FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHeroDetailWidget::Construct(const FArguments& InArgs)
{
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(50.f,0.f,0.f,0.f))
		[
			SNew(SBox)
			.HeightOverride(900)
			.WidthOverride(1800)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1)
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
				+SHorizontalBox::Slot()
				.FillWidth(1)
				[
					SNew(SScrollBox)
					.ScrollBarAlwaysVisible(true)
					+SScrollBox::Slot()
					[
						SNew(SBorder)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.Padding(0.f)
						.
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
				+SHorizontalBox::Slot()
				.FillWidth(4)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.FillHeight(3)
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
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
