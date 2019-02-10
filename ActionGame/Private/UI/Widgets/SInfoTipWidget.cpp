// Fill out your copyright notice in the Description page of Project Settings.

#include "SInfoTipWidget.h"
#include "SlateOptMacros.h"

using FPaddingParam = TAttribute<FMargin>;

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInfoTipWidget::Construct(const FArguments& InArgs)
{
	FSlateBrush* BorderBrush = new FSlateBrush();
	BorderBrush->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));

	FSlateBrush* TextBrush = new FSlateBrush();
	TextBrush->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.3f));
	FMargin ShowPos = InArgs._ShowPos;

	FPaddingParam ::FGetter TransformGetter;
	TransformGetter.BindLambda([ShowPos, this]() {
		const float CurLerp = AnimHandle.GetLerp();
		SetRenderOpacity(CurLerp);
		return FMargin(0.f, - 500.f + 500.f*CurLerp, 0.f, -100.f);
		});
	FPaddingParam  TransformParam;
	TransformParam.Bind(TransformGetter);

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Left)
	.VAlign(EVerticalAlignment::VAlign_Top)
	[
		SNew(SBox)
		.HeightOverride(400.f)
		.WidthOverride(500.f)
		.Padding(TransformParam)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(0.5f)
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
			]
			+SHorizontalBox::Slot()
			.FillWidth(20.f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1.5f)
				[
					SNew(SBorder)
					.BorderImage(BorderBrush)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString(TEXT("Tips"))))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),22))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.f,0.f,0.f,1.f)))
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(10.f)
				[
					SNew(SBorder)
					.BorderImage(TextBrush)
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
						.Text(FText::FromString(FString(TEXT("Contentffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"))))
						.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),22))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.f,0.f,0.f,1.f)))
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(0.5f)
				[
					SNew(SBorder)
					.BorderImage(BorderBrush)
				]
			]
			+SHorizontalBox::Slot()
			.FillWidth(0.5f)
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
			]
		]
		
	];

	SetupAnimation();
}

void SInfoTipWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AnimHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);

	AnimSequence.Play(this->AsShared());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION