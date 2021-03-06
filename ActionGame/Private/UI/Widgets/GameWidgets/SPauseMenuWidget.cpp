﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SPauseMenuWidget.h"
#include "SlateOptMacros.h"
#include <SBackgroundBlur.h>
#include <Engine/Texture2D.h>
#include <Engine/World.h>
#include <Engine/Engine.h>
#include "HAIAIMIHelper.h"
#include <SlateTypes.h>
#include "../Styles/FActionGameStyle.h"
#include "Kismet/GameplayStatics.h"
#include "ActionGameLoadingScreen.h"

#define LOCTEXT_NAMESPACE "ActionGame.UI.PauseMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerController = InArgs._OwnerController;
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	FontInfo = FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("UI/Fonts/NanumGothic.ttf"), 25);
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SBackgroundBlur)
			.BlurStrength(10)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.RenderTransform(FSlateRenderTransform(FVector2D(0.f,-300.f)))
			.Font(FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("UI/Fonts/NanumGothic.ttf"), 80))
			.ColorAndOpacity_Lambda([&]() {
				if (OwnerController.IsValid())
				{
					return OwnerController->bIsWon ? FSlateColor(FLinearColor(0.f,1.f,0.94f,1.f)) : FSlateColor(FLinearColor(1.f,0.f,0.f,1.f));
				}
				return FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			})
			.Text_Lambda([&]() {
				if(OwnerController.IsValid())
				{
					return OwnerController->bIsWon ? LOCTEXT("Win", "胜利！") : LOCTEXT("Lose", "失败！");
				}
				return LOCTEXT("Null Player!", "Null Player!");
			})
			.Visibility_Lambda([&]() {
				if (OwnerController.IsValid())
				{
					return OwnerController->bIsGameEnd ? EVisibility::Visible : EVisibility::Hidden;
				}
				return EVisibility::Hidden;
			})
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(&UIStyle->HeroItemBorder)
				[
					SNew(SBox)
					.HeightOverride(80.f)
					.WidthOverride(320.f)
					[
						SAssignNew(ContinueButton, SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.OnPressed(this, &SPauseMenuWidget::ContinueGame)
						.ButtonStyle(ButtonStyle)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Continue","继续游戏"))
							.Font(FontInfo)
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
				.Visibility_Lambda([&]() {
					if (OwnerController.IsValid())
					{
						return OwnerController->bIsGameEnd ? EVisibility::Hidden : EVisibility::Visible;
					}
					return EVisibility::Visible;
				})
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(&UIStyle->HeroItemBorder)
				[
					SNew(SBox)
					.HeightOverride(80.f)
					.WidthOverride(320.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						.OnPressed_Lambda([&]() {
							if (OwnerController.IsValid())
							{
								UGameplayStatics::OpenLevel(OwnerController.Get(), TEXT("/Game/GameLevels/GameMap"));
								IActionGameLoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<IActionGameLoadingScreenModule>("ActionGameLoadingScreen");
								if (LoadingScreenModule != nullptr)
								{
									LoadingScreenModule->StartInGameLoadingScreen();
								}
							}
						})
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Restart","重新开始"))
							.Font(FontInfo)
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(&UIStyle->HeroItemBorder)
				[
					SNew(SBox)
					.HeightOverride(80.f)
					.WidthOverride(320.f)	
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonStyle(ButtonStyle)
						.OnPressed_Lambda([&]() {
							if (OwnerController.IsValid())
							{
								UGameplayStatics::OpenLevel(OwnerController.Get(), TEXT("/Game/GameLevels/MenuLevel"));
							}
						})
						[
							SNew(STextBlock)
							.Text(LOCTEXT("BackToMenu","返回菜单"))
							.Font(FontInfo)
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(&UIStyle->HeroItemBorder)
				[
					SNew(SBox)
					.HeightOverride(80.f)
					.WidthOverride(320.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.OnPressed(this, &SPauseMenuWidget::QuitGame)
						.ButtonStyle(ButtonStyle)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Quit","退出游戏"))
							.Font(FontInfo)
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
		]
	];
}

void SPauseMenuWidget::SetContinueButtonEnable(bool bEnabled)
{
	ContinueButton->SetEnabled(bEnabled);
}

void SPauseMenuWidget::ContinueGame()
{
	if (OwnerController.IsValid())
		OwnerController->SetPause(false);
}

void SPauseMenuWidget::QuitGame()
{
	if (OwnerController.IsValid())
	{
		OwnerController->ConsoleCommand("quit");
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE