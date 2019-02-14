// Fill out your copyright notice in the Description page of Project Settings.

#include "SSettingsWidget.h"
#include "SlateOptMacros.h"
#include "SBackgroundBlur.h"
#include "../Styles/FActionGameStyle.h"
#include "SSelectBoxWidget.h"
#include "SMainMenuWidget.h"
#include "SDividingLineWidget.h"
#include "GameFramework/GameUserSettings.h"

#define GETGRAPHICLEVEL(EffectName)\
	UGameUserSettings::GetGameUserSettings()->Get##EffectName##Quality()

#define SETGRAPHICLEVEL(EffectName)\
	SSelectBoxWidget::FExecuteSelection EffectName;\
	EffectName##.BindLambda([&](float level) {\
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(level);\
			UGameUserSettings::GetGameUserSettings()->ApplySettings(false);\
			GConfig->Flush(false, GGameUserSettingsIni);\
		});

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSettingsWidget::Construct(const FArguments& InArgs)
{
	using FPaddingParam = TAttribute<FMargin>;
	OwnerHUD = InArgs._OwnerHUD;
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	TagButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("TagButtonStyle"));
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.2f));

	SETGRAPHICLEVEL(SelectionDelegate)

	TArray<FString> TempText = { TEXT("Low"),TEXT("Medium"),TEXT("High"),TEXT("Ultra") };
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
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("设置")))
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),40))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						.WidthOverride(200)
						[
							SNew(SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("画面设置")))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(10.f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SScrollBox)
						.ScrollBarAlwaysVisible(true)
						+SScrollBox::Slot()
						.Padding(1.f)
						[
							SNew(SDividingLineWidget)
							.CategoryName(FText::FromString(TEXT("画面渲染")))
						]
						+SScrollBox::Slot()
						.Padding(1.f)
						[
							SNew(SSelectBoxWidget)
							.SelectName(FText::FromString(TEXT("抗锯齿")))
							.SelectContent(TempText)
							.CurSelection(GETGRAPHICLEVEL(AntiAliasing))
							.ExecuteSelection(SelectionDelegate)
						]
						+SScrollBox::Slot()
						.Padding(1.f)
						[
							SNew(SSelectBoxWidget)
							.SelectName(FText::FromString(TEXT("阴影质量")))
							.SelectContent(TempText)
							.CurSelection(2.f)
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
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
				+SVerticalBox::Slot()
				.FillHeight(1.5f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SButton)
						.OnPressed(this, &SSettingsWidget::BackToPrevious)
						.ButtonStyle(BackButtonStyle)
					]
					+SHorizontalBox::Slot()
					.FillWidth(15.f)
					[
						SNew(SBorder)
					]
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
	AnimSequence.Reverse();

	if(OwnerHUD.IsValid())
	{
		auto PreMenu = OwnerHUD->MainMenu;
		PreMenu->BackToShow();
	}
}

void SSettingsWidget::BackToShow()
{
	AnimSequence.Reverse();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
