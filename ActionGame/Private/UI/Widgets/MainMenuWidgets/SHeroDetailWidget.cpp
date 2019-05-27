// Fill out your copyright notice in the Description page of Project Settings.

#include "SHeroDetailWidget.h"
#include "SlateOptMacros.h"
#include "SMainMenuWidget.h"
#include "TimerManager.h"
#include "HAIAIMIHelper.h"
#include "UI/Styles/FActionGameStyle.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

using FPaddingParam = TAttribute<FMargin>;
using FRenderTransformParam = TAttribute<TOptional<FSlateRenderTransform>>;

#define LOCTEXT_NAMESPACE "ActionGame.UI.HeroDetail"

const TArray<FText> AbilityButtonNames = { LOCTEXT("Overview","人物概况"), LOCTEXT("A","A"), LOCTEXT("B","B"), LOCTEXT("C","C"), LOCTEXT("D","D") };

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHeroDetailWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	PreWidget = InArgs._PreWidget;
	DetailPlatform = InArgs._DetailPlatform;
	TArray<FCharacterInfo> CharacterInfos;
	if (DetailPlatform.IsValid())
		CharacterInfos = DetailPlatform->CharInfos;
	SkinButtonHandles.SetNum(6);

	BorderBackground = MakeShareable(new FSlateBrush());
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.1f));
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	ButtonSelectedStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameSelectedButtonStyle"));
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	FPaddingParam::FGetter PadingGetter;
	PadingGetter.BindLambda([&]() {
		const float CurLerp = AnimHandle.GetLerp();
		return FMargin(1970.f - 1920.f*CurLerp, 40.f, -1870.f + 1920.f*CurLerp, 0.f);
		});
	FPaddingParam PaddingParam;
	PaddingParam.Bind(PadingGetter);

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
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
		.FillWidth(1)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(0.8f)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SBox)
				.WidthOverride_Lambda([&]() {
					return HAIAIMIHelper::GetMaxWidth();
				})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("HeroDetails", "人物细节"))
					.Font(FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("UI/Fonts/NanumGothic.ttf"), 40))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)))
				]	
			]
			+SVerticalBox::Slot()
			.FillHeight(0.3f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(300.f)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Heros","英雄"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),30))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(300.f)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Skins","皮肤"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),30))
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
					SNew(SBox)
					.WidthOverride(300.f)
					[
						SNew(SBorder)
						.BorderImage(BorderBackground.Get())
						[
							SAssignNew(HerosBar, SScrollBox)
							.ScrollBarAlwaysVisible(true)
						]
					]
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(300.f)
					[
						SNew(SBorder)
						.BorderImage(BorderBackground.Get())
						[
							SAssignNew(HeroSkinsBar, SScrollBox)
							.ScrollBarAlwaysVisible(true)
						]
					]
				]
				+SHorizontalBox::Slot()
				.FillWidth(1)
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
						SAssignNew(AbilityButtonContainer, SHorizontalBox)
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
						.OnPressed(this, &SHeroDetailWidget::BackToPrevious)
						.ButtonStyle(BackButtonStyle)
						.RenderTransform(FSlateRenderTransform(FVector2D(-100.f, 0.f)))
					]
				]
			]
		]
	];

	int32 ButtonNum = AbilityButtonNames.Num();
	AbilityButtons.SetNum(ButtonNum);
	for (int32 i = 0; i < ButtonNum; ++i)
	{
		AbilityButtonContainer->AddSlot()
		.FillWidth(1.f)
		[
			SAssignNew(AbilityButtons[i], SButton)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			.ButtonStyle(ButtonStyle)
			.OnUnhovered(this, &SHeroDetailWidget::CloseWidget)
			[
				SNew(STextBlock)
				.Text(AbilityButtonNames[i])
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
			]
		];
	}
	

	HeroButtons.SetNum(CharacterInfos.Num());
	for (int32 i = 0; i < CharacterInfos.Num(); ++i)
	{
		FSimpleDelegate Delegate;
		Delegate.BindLambda([i, this]() {
			ShowHeroSkinButtons(i);
			HeroButtons[i]->SetButtonStyle(ButtonSelectedStyle);
				for (int32 j = 0; j < HeroButtons.Num(); ++j)
				{
					if (j != i)
						HeroButtons[j]->SetButtonStyle(ButtonStyle);
				}
			});

		HerosBar->AddSlot()
		[
			SAssignNew(HeroButtons[i], SButton)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			.ButtonStyle(ButtonStyle)
			.OnPressed(Delegate)
			[
				SNew(STextBlock)
				.Text(CharacterInfos[i].CharName)
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
			]
		];
	}
	HeroButtons[0]->SetButtonStyle(ButtonSelectedStyle);
	ShowHeroSkinButtons(0);

	SetupAnimation();
}

void SHeroDetailWidget::BackToPrevious()
{
	AnimSequence.Reverse();

	if(OwnerHUD.IsValid())
	{
		auto PreMenu = OwnerHUD->MainMenu;
		PreMenu->BackToShow();
	}
}

void SHeroDetailWidget::BackToShow()
{
	AnimSequence.Reverse();
}

void SHeroDetailWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
	for (int32 i = 0; i < SkinButtonHandles.Num(); ++i)
	{
		SkinButtonHandles[i] = SkinButtonSequence.AddCurve(0.05f*i, 0.05f*i+0.05f, ECurveEaseFunction::CubicOut);
	}

	AnimSequence.Play(this->AsShared());
}

void SHeroDetailWidget::ShowHeroSkinButtons(int32 Index)
{
	if(HeroSkinsBar.IsValid() && DetailPlatform.IsValid())
	{
		SkinButtons.Reset();
		HeroSkinsBar->ClearChildren();

		const TArray<FCharacterInfo> CharacterInfos = DetailPlatform->CharInfos;
		const TArray<FText>& MeshNames = CharacterInfos[Index].MeshNames;
		SkinButtons.SetNum(MeshNames.Num());
		for (int32 i = 0; i < MeshNames.Num(); ++i)
		{
			FSimpleDelegate Delegate;

			Delegate.BindLambda([i, Index, this]() {
				if (DetailPlatform.IsValid())
					DetailPlatform->SetCharacterMesh(Index, i);
				SkinButtons[i]->SetButtonStyle(ButtonSelectedStyle);
				for (int32 j = 0; j < SkinButtons.Num(); ++j)
				{
					if (j != i)
						SkinButtons[j]->SetButtonStyle(ButtonStyle);
				}
			});

			FRenderTransformParam::FGetter TransformGetter;
			TransformGetter.BindLambda([i,this]() {
				const float CurLerp = SkinButtonHandles[i].GetLerp();
				SkinButtons[i]->SetRenderOpacity(CurLerp);
				return FSlateRenderTransform(FVector2D(0.f, (-50.f - i * 50.f)*(1 - CurLerp)));
				});
			FRenderTransformParam TransformParam;
			TransformParam.Bind(TransformGetter);
		
			HeroSkinsBar->AddSlot()
			[
				SAssignNew(SkinButtons[i], SButton)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.ButtonStyle(ButtonStyle)
				.OnPressed(Delegate)
				.RenderTransform(TransformParam)
				[
					SNew(STextBlock)
					.Text(MeshNames[i])
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"), 34))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
			];
		}
		SkinButtons[0]->SetButtonStyle(ButtonSelectedStyle);
		DetailPlatform->SetCharacterMesh(Index, 0);

		
		if (DetailPlatform.IsValid())
		{
			TArray<FCharacterInfo>& CharacterInfos_1 = DetailPlatform->CharInfos;
			TArray<FOnClicked> AbilityButtonDelegates;
			AbilityButtonDelegates.SetNum(4);
			for (int32 i = 0; i < 4; ++i)
			{
				FOnClicked AbilityButtonDelegate;
				AbilityButtonDelegate.BindLambda([i, Index, this]() {
					if (DetailPlatform.IsValid())
						DetailPlatform->PlayMontage(Index, i);
					return FReply::Handled();
					});
				AbilityButtons[i + 1]->SetOnClicked(AbilityButtonDelegate);
			}

			for (int32 i = 0; i < AbilityButtons.Num(); ++i)
			{
				
				FSimpleDelegate AbilityButtonDelegate;
				AbilityButtonDelegate.BindLambda([i, Index, CharacterInfos_1, this]() {
					ShowTips(CharacterInfos_1[Index].HeroDetails[i]);
					});
				AbilityButtons[i]->SetOnHovered(AbilityButtonDelegate);
			}
		}

		SkinButtonSequence.Play(this->AsShared());
	}
}

void SHeroDetailWidget::ShowTips(const FText& ShowText)
{
	if (TipWidget.IsValid())return;

	FPaddingParam::FGetter PadingGetter;
	PadingGetter.BindLambda([&]() {
			if(OwnerHUD.IsValid() && OwnerHUD->GetOwningPlayerController())
			{
				FVector2D MousePos, ViewportSize;
				OwnerHUD->GetOwningPlayerController()->GetMousePosition(MousePos.X, MousePos.Y);

				MousePos.Y -= 100.f;
				MousePos.X += 60.f;
				if (GEngine)
				{
					GEngine->GameViewport->GetViewportSize(ViewportSize);
					const float NormalWidth = ViewportSize.Y*(16.f / 9.f);
					if ((ViewportSize.X - MousePos.X)*1920.f / NormalWidth < 500.f)
						MousePos.X -= 620.f*(ViewportSize.Y / 1080.f);
				}
				MousePos = HAIAIMIHelper::ConvertToNormalCoord(MousePos);   //获取鼠标位置

				float Bottom = 0.f;
				if (MousePos.Y + 400.f > 1080.f)
				Bottom = 1080.f - MousePos.Y - 400.f;
				return FMargin(MousePos.X, MousePos.Y, 0.f, Bottom);
			}
			return FMargin();
		});
	FPaddingParam PaddingParam;
	PaddingParam.Bind(PadingGetter);

	if(OwnerHUD.IsValid())
	{
		auto PreMenu = OwnerHUD->MainMenu;
		PreMenu->GetMenuOverlay()->AddSlot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(PaddingParam)
		[
			SAssignNew(TipWidget, SBox)
			.HeightOverride(400.f)
			.WidthOverride(500.f)
			[
				SNew(SInfoTipWidget)
				.TipContent(ShowText)
			]
		];
	}
}

void SHeroDetailWidget::CloseWidget()
{
	if (!TipWidget.IsValid())return;
	if (OwnerHUD.IsValid())
	{
		auto PreMenu = OwnerHUD->MainMenu;
		PreMenu->GetMenuOverlay()->RemoveSlot(TipWidget.ToSharedRef());
		TipWidget.Reset();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE