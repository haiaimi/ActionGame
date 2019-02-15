// Fill out your copyright notice in the Description page of Project Settings.

#include "SSettingsWidget.h"
#include "SlateOptMacros.h"
#include "SBackgroundBlur.h"
#include "../Styles/FActionGameStyle.h"
#include "SSelectBoxWidget.h"
#include "SMainMenuWidget.h"
#include "SDividingLineWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "CoreGlobals.h"

#define GETGRAPHICLEVEL(EffectName)\
	UGameUserSettings::GetGameUserSettings()->Get##EffectName##Quality()

static const TArray<FString> FullScreenModeText = { TEXT("全屏"),TEXT("无边框"),TEXT("窗口") };

static const TArray<FString> ResolutionText = { TEXT("1280 x 720"),TEXT("1920 x 1080"),TEXT("2560 x 1440") };

static const TArray<FString> OnOffText = { TEXT("关闭"),TEXT("开启")};

static const TArray<FString> FrameLimitText = { TEXT("30"),TEXT("60"), TEXT("90"), TEXT("144"), TEXT("200")};

static const TArray<FString> GraphicLevelText = { TEXT("Low"),TEXT("Medium"),TEXT("High"),TEXT("Ultra") };

static const TArray<float> FrameLimitNum = { 30.f,60.f,90.f,144.f,200.f };

static const TArray<FIntPoint> Resolutions = { FIntPoint(1280, 720), FIntPoint(1920, 1080), FIntPoint(2560, 1440) };


#define SETGRAPHICLEVEL(EffectName)\
	SSelectBoxWidget::FExecuteSelection EffectName;\
	EffectName##.BindLambda([&](float level) {\
			UGameUserSettings::GetGameUserSettings()->Set##EffectName##Quality(level);\
			UGameUserSettings::GetGameUserSettings()->ApplySettings(false);\
			GConfig->Flush(false, GGameUserSettingsIni);\
		});

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSettingsWidget::Construct(const FArguments& InArgs)
{
	using FPaddingParam = TAttribute<FMargin>;
	OwnerHUD = InArgs._OwnerHUD;
	OwnerController = InArgs._OwnerController;
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	TagButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("TagButtonStyle"));
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.2f));
	SettingTagButtons.SetNum(2);
	SettingButtonHandles.SetNum(15);
	ButtonTransformParams.SetNum(15);

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
				.FillHeight(2)
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
							SAssignNew(SettingTagButtons[0], SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.OnPressed(this, &SSettingsWidget::ShowGraphicSettingList)
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("画面设置")))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						.WidthOverride(200)
						[
							SAssignNew(SettingTagButtons[1], SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.OnPressed(this, &SSettingsWidget::ShowOperationSettingList)
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("操作设置")))
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
						SAssignNew(SettingList, SScrollBox)
						.ScrollBarAlwaysVisible(true)
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
						.RenderTransform(FSlateRenderTransform(FVector2D(-100.f, 0.f)))
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
	ShowGraphicSettingList();
}

void SSettingsWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
	for (int32 i = 0; i < 15; ++i)
	{
		SettingButtonHandles[i] = AnimSequence.AddCurve(0.4f + i * 0.08f, 0.08f*i, ECurveEaseFunction::CubicOut);

		FRenderTransformParam::FGetter TransformGetter;
		TransformGetter.BindLambda([i,this]() {
			const float CurLerp = SettingButtonHandles[i].GetLerp();
			return FSlateRenderTransform(FVector2D(0.f, (-60.f - i * 60.f)*(1 - CurLerp)));
			});

		ButtonTransformParams[i].Bind(TransformGetter);
	}

	AnimSequence.Play(this->AsShared());
}

void SSettingsWidget::BackToPrevious()
{
	AnimSequence.Play(this->AsShared(), false, 0.5f);
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

void SSettingsWidget::SetTagButtonHighlight(int32 ButtonIndex)
{
	for (int32 i = 0; i < SettingTagButtons.Num(); ++i)
	{
		if (i == ButtonIndex)
			SettingTagButtons[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		else
			SettingTagButtons[i]->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void SSettingsWidget::ShowGraphicSettingList()
{
	if(SettingList.IsValid())
	{
		SetTagButtonHighlight(0);
		SettingList->ClearChildren();
		int32 AnimIndex = 0;

		SETGRAPHICLEVEL(AntiAliasing)
		SETGRAPHICLEVEL(Shadow)
		SETGRAPHICLEVEL(PostProcessing)
		SETGRAPHICLEVEL(ViewDistance)
		SETGRAPHICLEVEL(Texture)
		SETGRAPHICLEVEL(VisualEffect)
		SETGRAPHICLEVEL(Foliage)

		SSelectBoxWidget::FExecuteSelection FullScreenMode;
		FullScreenMode.BindLambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetFullscreenMode((EWindowMode::Type)((int32)level));
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			});


		SSelectBoxWidget::FExecuteSelection VSync;
		VSync.BindLambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(level == 1.f);
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			});

		SSelectBoxWidget::FExecuteSelection FrameLimit;
		FrameLimit.BindLambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(FrameLimitNum[(int32)level]);
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			});

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SDividingLineWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.CategoryName(FText::FromString(TEXT("基础设置")))
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("全屏模式")))
			.SelectContent(FullScreenModeText)
			.CurSelection((int32)UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())
			.ExecuteSelection_Lambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetFullscreenMode((EWindowMode::Type)((int32)level));
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			})
		];

		int32 ResIndex = 0;
		Resolutions.Find(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), ResIndex);

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("全屏分辨率")))
			.SelectContent(ResolutionText)
			.CurSelection(ResIndex != -1 ? ResIndex : 1)
			.ExecuteSelection_Lambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetScreenResolution(Resolutions[(int32)level]);
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				})
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("垂直同步")))
			.SelectContent(OnOffText)
			.CurSelection((int32)UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled())
			.ExecuteSelection_Lambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(level == 1.f);
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			})
		];

		int32 FrameLimitIndex = 0;
		FrameLimitNum.Find(UGameUserSettings::GetGameUserSettings()->GetFrameRateLimit(), FrameLimitIndex);

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("最大帧率")))
			.SelectContent(FrameLimitText)
			.CurSelection(FrameLimitIndex != -1 ? FrameLimitIndex : 4)
			.ExecuteSelection_Lambda([&](float level) {
				UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(FrameLimitNum[(int32)level]);
				UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
				GConfig->Flush(false, GGameUserSettingsIni);
			})
			.IsEnabled_Lambda([&]() { return !UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled(); })
		];


		bool Ref;
		GConfig->GetBool(TEXT("haiaimi"), TEXT("MotionBlur"), Ref, GGameIni);

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("动态模糊")))
			.SelectContent(OnOffText)
			.CurSelection((int32)Ref)
			.ExecuteSelection_Lambda([&](float level) {
				GConfig->SetBool(TEXT("haiaimi"), TEXT("MotionBlur"), level == 1.f, GGameIni);
				GConfig->Flush(false, GGameIni);
				int32 MotionBlurLevel = level == 1.f ? 2 : 0;
	
				if (OwnerController.IsValid())
					OwnerController->ConsoleCommand(FString::Printf(TEXT("r.MotionBlurQuality %d"), MotionBlurLevel));
			})
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SDividingLineWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.CategoryName(FText::FromString(TEXT("画面渲染")))
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("抗锯齿")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(AntiAliasing))
			.ExecuteSelection(AntiAliasing)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("阴影质量")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(Shadow))
			.ExecuteSelection(Shadow)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("后期处理")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(PostProcessing))
			.ExecuteSelection(PostProcessing)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("视图距离")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(ViewDistance))
			.ExecuteSelection(ViewDistance)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("纹理质量")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(Texture))
			.ExecuteSelection(Texture)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("特效")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(VisualEffect))
			.ExecuteSelection(VisualEffect)
		];

		SettingList->AddSlot()
		.Padding(1.f)
		[
			SNew(SSelectBoxWidget)
			.RenderTransform(ButtonTransformParams[AnimIndex++])
			.SelectName(FText::FromString(TEXT("植被")))
			.SelectContent(GraphicLevelText)
			.CurSelection(GETGRAPHICLEVEL(Foliage))
			.ExecuteSelection(Foliage)
		];

		if (AnimSequence.GetSequenceTime() >= 0.5f)
			AnimSequence.Play(this->AsShared(), false, 0.5f);
	}
}

void SSettingsWidget::ShowOperationSettingList()
{
	if (SettingList.IsValid())
	{
		SetTagButtonHighlight(1);
		SettingList->ClearChildren();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION