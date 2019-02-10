// Fill out your copyright notice in the Description page of Project Settings.

#include "SHeroDetailWidget.h"
#include "SlateOptMacros.h"
#include "SMainMenuWidget.h"
#include "TimerManager.h"
#include "HAIAIMIHelper.h"
#include "UI/Styles/FActionGameStyle.h"

using FPaddingParam = TAttribute<FMargin>;
using FRenderTransformParam = TAttribute<TOptional<FSlateRenderTransform>>;

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

	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.1f));
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	ButtonSelectedStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameSelectedButtonStyle"));
	FPaddingParam::FGetter PadingGetter;
	PadingGetter.BindLambda([&]() {
		const float CurLerp = AnimHandle.GetLerp();
		return FMargin(1970.f - 1920.f*CurLerp, 40.f, -1870.f + 1920.f*CurLerp, 0.f);
		});
	FPaddingParam PaddingParam;
	PaddingParam.Bind(PadingGetter);

	TArray<FSimpleDelegate> AbilityButtonDelegates;
	AbilityButtonDelegates.SetNum(3);
	for (int32 i = 0; i < 3; ++i)
	{
		AbilityButtonDelegates[i].BindLambda([i, this]() {
			if (DetailPlatform.IsValid())
				DetailPlatform->PlayMontage(i);
			});
	}

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
						SAssignNew(HerosBar, SScrollBox)
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
					]
					
				]
				+SHorizontalBox::Slot()
				.FillWidth(1)
				[
					SNew(SBorder)
					.BorderImage(BorderBackground)
					[
						SAssignNew(HeroSkinsBar, SScrollBox)
						.ScrollBarAlwaysVisible(true)
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
							.OnPressed(AbilityButtonDelegates[0])
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
							.OnPressed(AbilityButtonDelegates[1])
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
							.OnPressed(AbilityButtonDelegates[2])
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
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::QuadOut);
	for (int32 i = 0; i < SkinButtonHandles.Num(); ++i)
	{
		SkinButtonHandles[i] = SkinButtonSequence.AddCurve(0.05f*i, 0.05f*i+0.05f, ECurveEaseFunction::QuadOut);
	}

	AnimSequence.Play(this->AsShared());
}

void SHeroDetailWidget::ShowHeroSkinButtons(int32 Index)
{
	if(HeroSkinsBar.IsValid() && DetailPlatform.IsValid())
	{
		SkinButtons.Reset();
		HeroSkinsBar->ClearChildren();
		HeroSkinsBar->AddSlot()
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
		];

		const TArray<FCharacterInfo> CharacterInfos = DetailPlatform->CharInfos;
		const TArray<FText>& MeshNames = CharacterInfos[Index].MeshNames;
		SkinButtons.SetNum(MeshNames.Num());
		for (int32 i = 0; i < MeshNames.Num(); ++i)
		{
			FSimpleDelegate Delegate;

			Delegate.BindLambda([i, this]() {
				if (DetailPlatform.IsValid())
					DetailPlatform->SetCharacterMesh(i);
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
		DetailPlatform->SetCharacterMesh(0);

		SkinButtonSequence.Play(this->AsShared());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
