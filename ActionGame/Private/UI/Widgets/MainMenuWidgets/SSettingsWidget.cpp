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
#include "HAIAIMIHelper.h"



#define GETGRAPHICLEVEL(EffectName)\
	UGameUserSettings::GetGameUserSettings()->Get##EffectName##Quality()

#define SETGRAPHICLEVEL(EffectName)\
	SSelectBoxWidget::FExecuteSelection EffectName;\
	EffectName##.BindLambda([&](float level) {\
			UGameUserSettings::GetGameUserSettings()->Set##EffectName##Quality(level);\
			UGameUserSettings::GetGameUserSettings()->ApplySettings(false);\
			GConfig->Flush(false, GGameUserSettingsIni);\
		});

static const int32 MaxSettingCounts = 20;
#define LOCTEXT_NAMESPACE "ActionGame.UI.Setting"

static const TArray<FText> FullScreenModeText = { LOCTEXT("Fullscreen","全屏"),LOCTEXT("WindowedFullscreen","无边框"),LOCTEXT("Windowed","窗口") };

static const TArray<FText> ResolutionText = { LOCTEXT("720p","1280 x 720"),LOCTEXT("1080p","1920 x 1080"),LOCTEXT("1440p","2560 x 1440") };

static const TArray<FText> OnOffText = { LOCTEXT("Close","关闭"),LOCTEXT("Open","开启")};

static const TArray<FText> FrameLimitText = { LOCTEXT("Frame30","30"),LOCTEXT("Frame60","60"), LOCTEXT("Frame90","90"), LOCTEXT("Frame144","144"), LOCTEXT("Frame200","200")};

static const TArray<FText> GraphicLevelText = { LOCTEXT("Low","低"),LOCTEXT("Medium","中"),LOCTEXT("High","高"),LOCTEXT("Ultra","极致") };

static const TArray<FText> LanguageText = { LOCTEXT("Chinese","中文"),LOCTEXT("English","English") };

static const TArray<float> FrameLimitNum = { 30.f,60.f,90.f,144.f,200.f };

static const TArray<FIntPoint> Resolutions = { FIntPoint(1280, 720), FIntPoint(1920, 1080), FIntPoint(2560, 1440) };

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSettingsWidget::Construct(const FArguments& InArgs)
{
	using FPaddingParam = TAttribute<FMargin>;
	OwnerHUD = InArgs._OwnerHUD;
	OwnerController = InArgs._OwnerController;
	NewResolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	NewFullscreenMode = UGameUserSettings::GetGameUserSettings()->GetFullscreenMode();
	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));
	BackButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackButtonStyle"));
	TagButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("TagButtonStyle"));
	ApplySettingButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ApplySettingButtonStyle"));
	BorderBackground = MakeShareable(new FSlateBrush());
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
	SettingTagButtons.SetNum(3);
	SettingButtonHandles.SetNum(MaxSettingCounts);
	ButtonTransformParams.SetNum(MaxSettingCounts);
	SettingBorders.SetNum(MaxSettingCounts);
	
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
				.FillHeight(2)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SBox)
					.WidthOverride_Lambda([&]() {
						return HAIAIMIHelper::GetMaxWidth();
					})
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Setting", "设置"))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir() / TEXT("UI/Fonts/NanumGothic.ttf"), 40))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)))
					]
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
						[
							SAssignNew(SettingTagButtons[0], SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.OnPressed(this, &SSettingsWidget::ShowGraphicSettingList)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("PictureSetting","画面设置"))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(50)
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						[
							SAssignNew(SettingTagButtons[1], SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.OnPressed(this, &SSettingsWidget::ShowCommonSetting)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("CommonSetting","常规设置"))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(50)
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.HeightOverride(70)
						[
							SAssignNew(SettingTagButtons[2], SButton)
							.ButtonStyle(TagButtonStyle)
							.HAlign(EHorizontalAlignment::HAlign_Left)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.OnPressed(this, &SSettingsWidget::ShowOperationSettingList)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("OperationSetting","操作设置"))
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
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(900.f)
						[
							SAssignNew(SettingList, SScrollBox)
							.ScrollBarAlwaysVisible(true)
						]
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.FillHeight(2)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SAssignNew(SettingTitle, STextBlock)
							.AutoWrapText(true)
							.Justification(ETextJustify::Center)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),40))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
						+SVerticalBox::Slot()
						.FillHeight(3)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.VAlign(EVerticalAlignment::VAlign_Top)
						[
							SAssignNew(SettingDetails, STextBlock)
							.AutoWrapText(true)
							.Justification(ETextJustify::Center)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),20))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(1.5f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(100.f)
						[
							SNew(SButton)
							.OnPressed(this, &SSettingsWidget::BackToPrevious)
							.ButtonStyle(BackButtonStyle)
							.RenderTransform(FSlateRenderTransform(FVector2D(-100.f, 0.f)))
						]
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.Padding(30)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonStyle(ApplySettingButtonStyle)
							.Visibility_Lambda([&]() {
									if (NewResolution == UGameUserSettings::GetGameUserSettings()->GetScreenResolution() &&
										NewFullscreenMode == UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())return EVisibility::Hidden;
									return EVisibility::Visible;
								})
							.IsEnabled_Lambda([&]() {
									if (NewResolution == UGameUserSettings::GetGameUserSettings()->GetScreenResolution() &&
										NewFullscreenMode == UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())return false;
									return true;
								})
							.OnPressed_Lambda([&]() {
									UGameUserSettings::GetGameUserSettings()->SetScreenResolution(NewResolution);
									UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(NewFullscreenMode);
									UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
								})
							.OnHovered_Lambda([&]() {
									ApplySettingText->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)));
								})
							.OnUnhovered_Lambda([&]() {
									ApplySettingText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
								})
							[
								SAssignNew(ApplySettingText, STextBlock)
								.Text(LOCTEXT("ApplySetting","应用更改"))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),24))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
			]
		]
	];

	SetupAnimation();
	ShowGraphicSettingList();

	HAIAIMIHelper::Debug_ScreenMessage(HAIAIMIHelper::GetLocalization(), 5.f);
}

void SSettingsWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
	for (int32 i = 0; i < MaxSettingCounts; ++i)
	{
		SettingButtonHandles[i] = AnimSequence.AddCurve(0.4f + i * 0.05f, 0.04f*i, ECurveEaseFunction::CubicOut);

		FRenderTransformParam::FGetter TransformGetter;
		TransformGetter.BindLambda([i,this]() {
			const float CurLerp = SettingButtonHandles[i].GetLerp();
			if (SettingBorders[i].IsValid())
				SettingBorders[i]->SetRenderOpacity(CurLerp);
			return FSlateRenderTransform(FVector2D(0.f, -300.f*(1 - CurLerp)));
			});

		ButtonTransformParams[i].Bind(TransformGetter);
	}

	AnimSequence.Play(this->AsShared());
}

void SSettingsWidget::BackToPrevious()
{
	ClearScrollBox();
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
	ShowGraphicSettingList();
}

FReply SSettingsWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	HAIAIMIHelper::Debug_ScreenMessage(InKeyEvent.GetKey().ToString());

	return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
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
		SettingTitle->SetText(LOCTEXT("Picture","画面"));
		SettingDetails->SetText(LOCTEXT("PictureTag","画面输出选项"));

		SetTagButtonHighlight(0);
		ClearScrollBox();
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
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SDividingLineWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.CategoryName(LOCTEXT("BasicSetting","基础设置"))
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("FullScreenMode","全屏模式"))
				.SelectContent(FullScreenModeText)
				.CurSelection((int32)UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())
				.ExecuteSelection_Lambda([&](float level) {
					NewFullscreenMode = (EWindowMode::Type)((int32)level);
					})
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("FullScreenModeTip","全屏模式"));
					SettingDetails->SetText(LOCTEXT("FullScreenModeTipCont","游戏在显示器上的显示模式"));
					})
			]
		];

		int32 ResIndex = 0;
		Resolutions.Find(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), ResIndex);

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("FullScreenResolution","全屏分辨率"))
				.SelectContent(ResolutionText)
				.CurSelection(ResIndex != -1 ? ResIndex : 1)
				.ExecuteSelection_Lambda([&](float level) {
					NewResolution = Resolutions[(int32)level];
					})
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("FullScreenResolutionTip","全屏分辨率"));
					SettingDetails->SetText(LOCTEXT("FullScreenResolutionTip_Cont","显卡输出分辨率，越高画面越清晰，但是会影响性能，玩家要做好权衡"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("VSync","垂直同步"))
				.SelectContent(OnOffText)
				.CurSelection((int32)UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled())
				.ExecuteSelection_Lambda([&](float level) {
					UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(level == 1.f);
					UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
					GConfig->Flush(false, GGameUserSettingsIni);
				})
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("VSyncTip","垂直同步"));
					SettingDetails->SetText(LOCTEXT("VSyncTipCont","显卡输出显示器的固定帧率，可以缓解画面撕裂，但是会造成输入延迟"));
					})
			]
		];

		int32 FrameLimitIndex = 0;
		FrameLimitNum.Find(UGameUserSettings::GetGameUserSettings()->GetFrameRateLimit(), FrameLimitIndex);

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("MaxFrame", "最大帧率"))
				.SelectContent(FrameLimitText)
				.CurSelection(FrameLimitIndex != -1 ? FrameLimitIndex : 4)
				.ExecuteSelection_Lambda([&](float level) {
					UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(FrameLimitNum[(int32)level]);
					UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
					GConfig->Flush(false, GGameUserSettingsIni);
				})
				.IsEnabled_Lambda([&]() { return !UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled(); })
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("MaxFrameTip","最大帧率"));
					SettingDetails->SetText(LOCTEXT("MaxFrameTipCont","显卡输出的最高帧率，越高越流畅"));
					})
			]
		];


		bool Ref;
		GConfig->GetBool(TEXT("haiaimi"), TEXT("MotionBlur"), Ref, GGameIni);

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("MotionBlur","动态模糊"))
				.SelectContent(OnOffText)
				.CurSelection((int32)Ref)
				.ExecuteSelection_Lambda([&](float level) {
					GConfig->SetBool(TEXT("haiaimi"), TEXT("MotionBlur"), level == 1.f, GGameIni);
					GConfig->Flush(false, GGameIni);
					int32 MotionBlurLevel = level == 1.f ? 2 : 0;
					
					if (OwnerController.IsValid())
						OwnerController->ConsoleCommand(FString::Printf(TEXT("r.MotionBlurQuality %d"), MotionBlurLevel));
				})
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("MotionBlurTip","动态模糊"));
					SettingDetails->SetText(LOCTEXT("MotionBlurTipCont","帧间加入模糊效果，可以缓解帧数低时的卡顿感，帧数高不需要打开"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SDividingLineWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.CategoryName(LOCTEXT("GraphicRender","画面渲染"))
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("AntiAliasing","抗锯齿"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(AntiAliasing))
				.ExecuteSelection(AntiAliasing)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("AntiAliasingTip","抗锯齿"));
					SettingDetails->SetText(LOCTEXT("AntiAliasingTipCont","减少画面锯齿感，提升画面观感，使画面更加柔和"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("ShadowQuality","阴影质量"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(Shadow))
				.ExecuteSelection(Shadow)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("ShadowQualityTip","阴影质量"));
					SettingDetails->SetText(LOCTEXT("ShadowQualityTipCont","阴影渲染质量，越高效果越真实，也消耗性能"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("PostProcessing","后期处理"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(PostProcessing))
				.ExecuteSelection(PostProcessing)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("PostProcessingTip","后期处理"));
					SettingDetails->SetText(LOCTEXT("PostProcessingTipCont","游戏后处理"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("ViewDistance","视图距离"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(ViewDistance))
				.ExecuteSelection(ViewDistance)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("ViewDistanceTip","视图距离"));
					SettingDetails->SetText(LOCTEXT("ViewDistanceTipCont","可渲染的最大距离"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("TextureQuality","纹理质量"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(Texture))
				.ExecuteSelection(Texture)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("TextureQualityTip","纹理质量"));
					SettingDetails->SetText(LOCTEXT("TextureQualityTipCont","贴图的精细程度，越高越清晰，但是会占用较多性能和显存"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("VisualEffect","特效"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(VisualEffect))
				.ExecuteSelection(VisualEffect)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("VisualEffectTip","特效"));
					SettingDetails->SetText(LOCTEXT("VisualEffectTipCont","粒子效果"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("Foliage","植被"))
				.SelectContent(GraphicLevelText)
				.CurSelection(GETGRAPHICLEVEL(Foliage))
				.ExecuteSelection(Foliage)
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("FoliageTip","植被"));
					SettingDetails->SetText(LOCTEXT("FoliageTipQuality","植被渲染质量"));
					})
			]
		];

		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			.RenderOpacity(0.f)
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("Useless","Useless"))
				.SelectContent(GraphicLevelText)
				.CurSelection(0)
			]
		];


		if (AnimSequence.GetSequenceTime() >= 0.5f)
			AnimSequence.Play(this->AsShared(), false, 0.5f);
	}
}

void SSettingsWidget::ShowOperationSettingList()
{
	if (SettingList.IsValid())
	{
		SetTagButtonHighlight(2);
		ClearScrollBox();
	}
}

void SSettingsWidget::ShowCommonSetting()
{
	if (SettingList.IsValid())
	{
		SetTagButtonHighlight(1);
		ClearScrollBox();

		SettingTitle->SetText(LOCTEXT("CommonSettings","常规设置"));
		SettingDetails->SetText(LOCTEXT("CommonSettingsTag","游戏常规功能设置"));

		int32 AnimIndex = 0;
		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SDividingLineWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.CategoryName(LOCTEXT("User","用户"))
			]
		];

		FString Ref;
		GConfig->GetString(TEXT("haiaimi"), TEXT("Language"), Ref, GGameIni);
		HAIAIMIHelper::ChangeLocalization(Ref);
		SettingList->AddSlot()
		[
			SAssignNew(SettingBorders[AnimIndex], SBorder)
			.BorderImage(BorderBackground.Get())
			[
				SNew(SSelectBoxWidget)
				.RenderTransform(ButtonTransformParams[AnimIndex++])
				.SelectName(LOCTEXT("Language","语言"))
				.SelectContent(LanguageText)
				.CurSelection(Ref == "zh" ? 0 : 1)
				.ExecuteSelection_Lambda([&](float level) {
					HAIAIMIHelper::ChangeLocalization(level == 0.f ? "zh" : "en");
					GConfig->SetString(TEXT("haiaimi"), TEXT("Language"), level == 0.f ? TEXT("zh") : TEXT("en"), GGameIni);
					})
				.SelectionOnHovered_Lambda([&]() {
					SettingTitle->SetText(LOCTEXT("LanguageTip","语言"));
					SettingDetails->SetText(LOCTEXT("LanguageTipCont","游戏语言"));
					})
			]
		];
	}
}

void SSettingsWidget::ClearScrollBox()
{
	SettingList->ClearChildren();
	//SettingBorders.Reset(15);
	NewResolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	NewFullscreenMode = UGameUserSettings::GetGameUserSettings()->GetFullscreenMode();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE