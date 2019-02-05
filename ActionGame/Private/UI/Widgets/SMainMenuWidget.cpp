﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include "SlateTypes.h"
#include "UI/Styles/FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	AnimHandles.SetNum(4);
	TArray<TAttribute<TOptional<FSlateRenderTransform>>> RenderTransformDel;
	TAttribute<TOptional<FSlateRenderTransform>>::FGetter TransformGetter;
	RenderTransformDel.SetNum(4);

	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	for (int32 i = 0; i < 4; ++i)
	{
		TransformGetter.BindLambda([i, this]() {
			const float CurLerp = AnimHandles[i].GetLerp();
			FShear2D Shear(FVector2D(15.f, CurLerp*89.f - 89.f)/89);
			return FSlateRenderTransform(Shear, FVector2D(40.f + i * 40.f, 100.f));
		});
		
		RenderTransformDel[i].Bind(TransformGetter);
	}

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.RenderTransform(RenderTransformDel[0])
				.RenderTransformPivot(FVector2D(0.f, 1.f))
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(350.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						//.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						//.OnPressed(this, &SMainMenuWidget::LaunchGame)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("开始游戏"))))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.RenderTransform(RenderTransformDel[1])
				.RenderTransformPivot(FVector2D(0.f, 1.f))
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(350.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						//.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						//.OnPressed(this, &SMainMenuWidget::ShowRank)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("英雄介绍"))))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.RenderTransform(RenderTransformDel[2])
				.RenderTransformPivot(FVector2D(0.f, 1.f))
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(350.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						//.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						//.OnPressed(this, &SMainMenuWidget::QuitGame)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("退出游戏"))))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.RenderTransform(RenderTransformDel[3])
				.RenderTransformPivot(FVector2D(0.f, 1.f))
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(350.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						//.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						//.OnPressed(this, &SMainMenuWidget::QuitGame)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("关于"))))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
		]
	];
	
	SetupAnimation();
}

void SMainMenuWidget::SetupAnimation()
{
	MenuSecquence = FCurveSequence();
	float StartTime = 0.f;
	for (int32 i = 0; i < 4; ++i)
	{
		AnimHandles[i] = MenuSecquence.AddCurve(StartTime + 0.1f * i, 0.6f, ECurveEaseFunction::QuadOut);
	}

	MenuSecquence.Play(this->AsShared());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION