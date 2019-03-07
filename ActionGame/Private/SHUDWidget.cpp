// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUDWidget.h"
#include "SlateOptMacros.h"
#include "SAbilityIconWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHUDWidget::Construct(const FArguments& InArgs)
{
	Owner = InArgs._Owner;
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1.f)
			+SVerticalBox::Slot()
			.FillHeight(4.f)
			+SVerticalBox::Slot()
			.FillHeight(0.8f)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(AbilityContainer, SHorizontalBox)
				.RenderTransform_Lambda([&]() {
					const float CurLerp = AbilityIconHandle.GetLerp();
					return FSlateRenderTransform(FVector2D(0.f, (1 - CurLerp)*500.f));
				})
				+SHorizontalBox::Slot()
				.Padding(FMargin(0.f,0.f,20.f,0.f))
				[
					SNew(SAbilityIconWidget)
					.CoolingTime(5.f)
				]
				+SHorizontalBox::Slot()
				.Padding(FMargin(0.f,0.f,20.f,0.f))
				[
					SNew(SAbilityIconWidget)
					.RenderTransform(FSlateRenderTransform(FVector2D(0.f, -50.f)))
					.CoolingTime(6.f)
				]
				+SHorizontalBox::Slot()
				[
					SNew(SAbilityIconWidget)
					.CoolingTime(20.f)
				]
			]
		]
	];

	SetupAnimation();
	AnimSequence.Play(this->AsShared());
}

void SHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (Owner.IsValid())
	{

	}
}

void SHUDWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AbilityIconHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
