// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include "SlateTypes.h"
#include "UI/Styles/FActionGameStyle.h"
#include "GameActors/HeroDetailPlatform.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "HAIAIMIHelper.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerController = InArgs._OwnerController;
	OwnerHUD = InArgs._OwnerHUD;
	AnimHandles.SetNum(4);
	TArray<TAttribute<TOptional<FSlateRenderTransform>>> RenderTransformDel;
	TAttribute<TOptional<FSlateRenderTransform>>::FGetter TransformGetter;
	RenderTransformDel.SetNum(4);

	ButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ActionGameButtonStyle"));

	for (int32 i = 0; i < 4; ++i)
	{
		TransformGetter.BindLambda([i, this]() {
			const float CurLerp = AnimHandles[i].GetLerp();
			FShear2D Shear(FVector2D(15.f, 270.f*CurLerp-270.f)/90.f);
			MenuContainer->SetRenderOpacity(CurLerp);
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
		.RenderOpacity(0.f)
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
					.OnPressed(this, &SMainMenuWidget::HeroDetails)
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
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString(TEXT("关于"))))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),34))
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
					]
				]
			]
		]
	];

	/*MenuOverlay->AddSlot()
.Padding(FMargin(1000.f,800.f,0.f,0.f))
	[
		SNew(SBox)
		.HeightOverride(400.f)
		.WidthOverride(500.f)
		[
			SNew(SInfoTipWidget)
		]
		
	];*/

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
	for (int32 i = 0; i < 4; ++i)
	{
		AnimHandles[i] = MenuSequence.AddCurve(StartTime + 0.1f * i, 0.4f, ECurveEaseFunction::QuadOut);
	}

	MenuSequence.Play(this->AsShared());
}

void SMainMenuWidget::HeroDetails()
{
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
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


