// Fill out your copyright notice in the Description page of Project Settings.


#include "SInputBoxWidget.h"
#include "SlateOptMacros.h"
#include "SlateTypes.h"
#include "FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInputBoxWidget::Construct(const FArguments& InArgs)
{

	SelectBoxButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("SelectBoxButtonStyle"));
	SelectionOnHovered = InArgs._SelectionOnHovered;

	FText InitContent = InArgs._CurSelection;
	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SButton)
		.ButtonStyle(SelectBoxButtonStyle)
		.OnHovered_Lambda([&]() {
			if (SelectionName.IsValid())
				SelectionName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)));
			if (SelectionText.IsValid())
				SelectionText->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)));
			SelectionOnHovered.ExecuteIfBound();
			})
		.OnUnhovered_Lambda([&]() {
			if (SelectionName.IsValid())
				SelectionName->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
			if (SelectionText.IsValid())
				SelectionText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
			})
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(2)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(SelectionName, STextBlock)
				.Text(InArgs._SelectName)
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(SelectionText, STextBlock)
				.Text(InitContent)
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
