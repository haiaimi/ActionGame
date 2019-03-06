// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUDWidget.h"
#include "SlateOptMacros.h"
#include "SAbilityIconWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHUDWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.Padding(FMargin(500.f,800.f,0.f,0.f))
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SAbilityIconWidget)
				.CoolingTime(5.f)
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
