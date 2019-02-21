// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include "SlateTypes.h"
#include "UI/Styles/FActionGameStyle.h"
#include "GameActors/HeroDetailPlatform.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "HAIAIMIHelper.h"
#include "SSelectBoxWidget.h"
#include "SSettingsWidget.h"
#include "Kismet/GameplayStatics.h"

using FPaddingParam = TAttribute<FMargin>;

#define LOCTEXT_NAMESPACE "ActionGame.UI.MainMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerController = InArgs._OwnerController;
	OwnerHUD = InArgs._OwnerHUD;
	AnimHandles.SetNum(5);
	ButtonBorders.SetNum(5);
	TArray<TAttribute<TOptional<FSlateRenderTransform>>> RenderTransformDel;
	TAttribute<TOptional<FSlateRenderTransform>>::FGetter TransformGetter;
	RenderTransformDel.SetNum(5);

	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	for (int32 i = 0; i < 5; ++i)
	{
		TransformGetter.BindLambda([i, this]() {
			const float CurLerp = AnimHandles[i].GetLerp();
			FShear2D Shear(FVector2D(15.f, 270.f*CurLerp-270.f)/90.f);
			ButtonBorders[i]->SetRenderOpacity(CurLerp);
			return FSlateRenderTransform(Shear, FVector2D(40.f + i * 40.f, 100.f));
		});
		
		RenderTransformDel[i].Bind(TransformGetter);
	}

	ChildSlot
	[
		SAssignNew(MenuOverlay, SOverlay)
		+SOverlay::Slot()
		[
			SNew(SBorder)
		]
	];

	MenuOverlay->AddSlot()
	.HAlign(EHorizontalAlignment::HAlign_Left)
	.VAlign(EVerticalAlignment::VAlign_Top)
	[
		SAssignNew(MenuContainer, SVerticalBox)
		+SVerticalBox::Slot()
		[
			SAssignNew(ButtonBorders[0], SBorder)
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
					.OnPressed(this, &SMainMenuWidget::StartGame)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Start","开始游戏"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
		+SVerticalBox::Slot()
		[
			SAssignNew(ButtonBorders[1], SBorder)
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
					.OnPressed(this, &SMainMenuWidget::HeroDetails)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Heros","英雄介绍"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
		+SVerticalBox::Slot()
		[
			SAssignNew(ButtonBorders[2], SBorder)
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
					.OnPressed(this, &SMainMenuWidget::GameSetting)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Setting","游戏设置"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
		+SVerticalBox::Slot()
		[
			SAssignNew(ButtonBorders[3], SBorder)
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
					.OnPressed(this, &SMainMenuWidget::QuitGame)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Quit","退出游戏"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
		+SVerticalBox::Slot()
		[
			SAssignNew(ButtonBorders[4], SBorder)
			.RenderTransform(RenderTransformDel[4])
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
					[
						SNew(STextBlock)
						.Text(LOCTEXT("About","关于"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
	];

	SetupAnimation();

	if (OwnerController.IsValid())
		MenuShowActor = OwnerController->GetViewTarget();
}
	

void SMainMenuWidget::BackToShow()
{
	MenuSequence.Reverse();
	MenuContainer->SetEnabled(true);

	if (MenuShowActor && OwnerController.IsValid())
	{
		OwnerController->SetViewTargetWithBlend(MenuShowActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		OwnerController->SetCurWidget(OwnerHUD->MainMenu);  //要注意智能指针之间的引用，不能
	}
}

TSharedPtr<SOverlay>& SMainMenuWidget::GetMenuOverlay()
{
	return MenuOverlay;
}

void SMainMenuWidget::SetupAnimation()
{
	MenuSequence = FCurveSequence();
	float StartTime = 0.f;
	for (int32 i = 0; i < 5; ++i)
	{
		AnimHandles[i] = MenuSequence.AddCurve(StartTime + 0.1f * i, 0.3f, ECurveEaseFunction::QuadOut);
	}
	MenuSequence.Play(this->AsShared());
}

void SMainMenuWidget::StartGame()
{
	if (MenuSequence.IsInReverse())return;
	MenuSequence.Reverse();
	if (!StartGameWidget.IsValid())
	{
		MenuOverlay->AddSlot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Top)
			[
				SNew(SBox)
				.HeightOverride(1080)
				.WidthOverride(1920)
				.RenderTransform_Lambda([&]() {
				if (!StartGameWidget.IsValid())return FSlateRenderTransform(FVector2D::ZeroVector);
				const float CurLerp = StartGameWidget->GetCurAnimLerp();
				return FSlateRenderTransform(FVector2D(1920.f*(1.f - CurLerp), 0.f));
					})
				[
					SAssignNew(StartGameWidget, SStartGameWidget)
					.OwnerHUD(OwnerHUD)
				]
			];
	}
	else
		StartGameWidget->BackToShow();

	if (StartGameWidget.IsValid())
		OwnerController->SetCurWidget(StartGameWidget);

	/*if (OwnerController.IsValid())
		UGameplayStatics::OpenLevel(OwnerController.Get(), TEXT("/Game/GameLevels/GameMap"));*/
}

void SMainMenuWidget::HeroDetails()
{
	if (MenuSequence.IsInReverse())return;
	MenuSequence.Reverse();

	AHeroDetailPlatform* DetailPlatform = nullptr;
	if(OwnerController.IsValid())
	{
		for (TActorIterator<AHeroDetailPlatform> Iter(OwnerController->GetWorld()); Iter; ++Iter)
		{
			DetailPlatform = *Iter;
			break;
		}

		if(DetailPlatform)
		{
			OwnerController->SetViewTargetWithBlend(DetailPlatform, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			MenuContainer->SetEnabled(false);
		
			if (!HeroDetail.IsValid())
			{
				MenuOverlay->AddSlot(0)
				[
					SAssignNew(HeroDetail, SHeroDetailWidget)
					.OwnerHUD(OwnerHUD)
					//.PreWidget(TSharedPtr<SMainMenuWidget>(this))   //这样会造成智能指针循环引用
					.DetailPlatform(DetailPlatform)
				];
			}
			else
				HeroDetail->BackToShow();
			OwnerController->SetCurWidget(HeroDetail);
		}
	}
}

void SMainMenuWidget::GameSetting()
{
	if (MenuSequence.IsInReverse())return;
	MenuSequence.Reverse();

	if (!SettingWidget.IsValid())
	{
		FPaddingParam ::FGetter TransformGetter;
		TransformGetter.BindLambda([&]() {
			if (!SettingWidget.IsValid())return FMargin();
			const float CurLerp = SettingWidget->GetCurAnimLerp();
			SettingWidget->SetRenderOpacity(CurLerp);
			float Bottom = 400.f - 400.f*CurLerp;
			return FMargin(1980.f - 1980.f*CurLerp, 0.f, -1980.f + 1980.f*CurLerp, 0.f);
			});
		FPaddingParam  TransformParam;
		TransformParam.Bind(TransformGetter);

		MenuOverlay->AddSlot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SAssignNew(SettingsBox, SBox)
			.HeightOverride(1080)
			.WidthOverride(1920)
			.Padding(TransformParam)
			[
				SAssignNew(SettingWidget, SSettingsWidget)
				.OwnerHUD(OwnerHUD.IsValid() ? OwnerHUD : nullptr)
				.OwnerController(OwnerController.IsValid() ? OwnerController : nullptr)
			]
		];
	}
	else
		SettingWidget->BackToShow();
	if (SettingWidget.IsValid())
		OwnerController->SetCurWidget(SettingWidget);
}

void SMainMenuWidget::QuitGame()
{
	if (OwnerController.IsValid())
		OwnerController->ConsoleCommand("quit");
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
