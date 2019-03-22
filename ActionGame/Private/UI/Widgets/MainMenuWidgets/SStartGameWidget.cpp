// Fill out your copyright notice in the Description page of Project Settings.

#include "SStartGameWidget.h"
#include "SlateOptMacros.h"
#include "SBackgroundBlur.h"
#include "../Styles/FActionGameStyle.h"
#include "SMainMenuWidget.h"
#include "ActionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ActionGameLoadingScreen/Public/ActionGameLoadingScreen.h"
#include "ActionGameType.h"


#define LOCTEXT_NAMESPACE "ActionGame.UI.StartGame"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SStartGameWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	CurSelectIndex = 0;
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SBackgroundBlur)
		.BlurStrength(7.f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(100.f)
				.HeightOverride(1080.f)
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("GamePreparing","游戏准备"))
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),40))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
				+SVerticalBox::Slot()
				.FillHeight(0.5f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						.WidthOverride(400.f)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SelectHero","人物选择"))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						.WidthOverride(400.f)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SAssignNew(HeroName, STextBlock)
							.Text(HeroNameText[0])
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.98f,1.f,0.67f,1.f)))
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("SelectSkin","皮肤选择"))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(4.f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(HeroList, SScrollBox)
						.ScrollBarAlwaysVisible(true)
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.HeightOverride(100.f)
						]
						+ SVerticalBox::Slot()
						.FillHeight(1.f)
						[
							SAssignNew(SkinList, SSkinsScrollBox)
							.SkinImageBrushs(&UIStyle->Skins[0].Skins)
							.OwnerHUD(OwnerHUD)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							.HeightOverride(100.f)
						]
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(0.6f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(100.f)
						[
							SNew(SButton)
							.OnPressed(this, &SStartGameWidget::BackToPrevious)
							.ButtonStyle(BackButtonStyle)
							.RenderTransform(FSlateRenderTransform(FVector2D(-100.f, 0.f)))
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1)
					+SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(EHorizontalAlignment::HAlign_Right)
					[
						SNew(SBox)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.Padding(FMargin(0.f,0.f,50.f,0.f))
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ButtonStyle)
							.OnPressed(this, &SStartGameWidget::StartGame)
							.ContentPadding(FMargin(30.f,10.f,30.f,10.f))
							[
								SNew(STextBlock)
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),35))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
								.Text(LOCTEXT("StartGame","游戏开始"))
							]
						]
					]
				]
			]
		]
	];
	SetupAnimation();
	AddHeroHeads();
}

void SStartGameWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandles.SetNum(6);

	StartingPageHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);

	for (int32 i = 0; i < 6; ++i)
		AnimHandles[i] = AnimSequence.AddCurve(0.35f + 0.1f*i, 0.2f, ECurveEaseFunction::CubicOut);

	AnimSequence.Play(this->AsShared());
}

void SStartGameWidget::AddHeroHeads()
{
	int32 HeroNums = UIStyle->HeroHeadImages.Num();

	if(HeroList.IsValid())
	{
		for (int32 i = 0; i < 1 + HeroNums / 4; ++i)
		{
			TSharedPtr<SHorizontalBox> HeadContainer;
			TSharedPtr<SBox> TempBox =
			SNew(SBox)
			.WidthOverride(800.f)
			.HeightOverride(200.f)
			.RenderTransform_Lambda([i, this]() {
				const float CurLerp = AnimHandles[i].GetLerp();
				return FSlateRenderTransform(FVector2D(0.f, (5 - i)*200.f*(1 - CurLerp)));
				})

			[
				SAssignNew(HeadContainer, SHorizontalBox)
			];

			for (int32 j = 0; j < 4; ++j)
			{
				if (i * 4 + j < HeroNums)
				{
					HeadContainer->AddSlot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(200.f)
						.HeightOverride(200.f)
						[
							SNew(SHeroShowItem)
							.Image(&UIStyle->HeroHeadImages[i*4+j])
							.HoverScale(1.1f)
							.OnPressed_Lambda([i,j,this]() {
								if (i * 4 + j < UIStyle->Skins.Num())
									SkinList->SetSkinImages(&UIStyle->Skins[i * 4 + j].Skins);
								if (HeroName.IsValid() && i * 4 + j < HeroNameText.Num())
									HeroName->SetText(HeroNameText[i * 4 + j]);
								if(OwnerHUD.IsValid())
								{
									if (UActionGameInstance* MyInstance = OwnerHUD->GetGameInstance<UActionGameInstance>())
									{
										MyInstance->PlayerIndex = i * 4 + j;
									}
								}
								CurSelectIndex = i * 4 + j;
								})
							.IsEnabled_Lambda([i, j, this]() {
									return CurSelectIndex != i * 4 + j;
								})

						]
					];
				}
				else
					break;
			}

			HeroList->AddSlot()
			[
				TempBox.ToSharedRef()
			];
		}
	}
}

void SStartGameWidget::StartGame()
{
	if (OwnerHUD.IsValid())
	{
		UGameplayStatics::OpenLevel(OwnerHUD.Get(), TEXT("/Game/GameLevels/GameMap"));
		IActionGameLoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<IActionGameLoadingScreenModule>("ActionGameLoadingScreen");
		if( LoadingScreenModule != nullptr )
		{
			LoadingScreenModule->StartInGameLoadingScreen();
		}
	}
		
}

void SStartGameWidget::BackToPrevious()
{
	AnimSequence.Play(this->AsShared(), false, 0.5f);
	AnimSequence.Reverse();
	
	if(OwnerHUD.IsValid())
	{
		auto PreMenu = OwnerHUD->MainMenu;
		PreMenu->BackToShow();
	}
}

void SStartGameWidget::BackToShow()
{
	AnimSequence.Play(this->AsShared(), false);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE