// Fill out your copyright notice in the Description page of Project Settings.

#include "SSelectBoxWidget.h"
#include "SlateOptMacros.h"
#include "UI/Styles/FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSelectBoxWidget::Construct(const FArguments& InArgs)
{
	BackwardButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("BackwardButtonStyle"));
	ForwardButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("ForwardButtonStyle"));
	FSlateBrush* BorderBackground = new FSlateBrush();
	BorderBackground->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));

	SelectContent = InArgs._SelectContent;
	if (InArgs._CurSelection < SelectContent.Num())
		CurSelection = InArgs._CurSelection;
	else
		CurSelection = 0;
	FText InitContent = SelectContent.Num() == 0 ? FText::FromString(TEXT("Null Content")) : FText::FromString(SelectContent[CurSelection]);

	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SBorder)
		.BorderImage(BorderBackground)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(2)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(InArgs._SelectName)
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(LeftButton, SButton)
				.ButtonStyle(BackwardButtonStyle)
				.OnPressed(this, &SSelectBoxWidget::ToBack)
				.IsEnabled(CurSelection != 0)
			]
			+SHorizontalBox::Slot()
			.FillWidth(1)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(SelectionText, STextBlock)
				.Text(InitContent)
				.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(RightButton, SButton)
				.ButtonStyle(ForwardButtonStyle)
				.OnPressed(this, &SSelectBoxWidget::ToForward)
				.IsEnabled(CurSelection != SelectContent.Num() - 1)
			]
		]
	];
}

void SSelectBoxWidget::SetDelegate(FExecuteSelection InDelegate)
{
	ExecuteSelection = InDelegate;
}

void SSelectBoxWidget::ToBack()
{
	if (CurSelection > 0 && --CurSelection < SelectContent.Num())
	{
		SelectionText->SetText(FText::FromString(SelectContent[CurSelection]));
		if (CurSelection == 0)
			LeftButton->SetEnabled(false);
	}
		
	RightButton->SetEnabled(true);
	ExecuteSelection.ExecuteIfBound(CurSelection);
}

void SSelectBoxWidget::ToForward()
{
	if (CurSelection < SelectContent.Num() - 1 && ++CurSelection < SelectContent.Num())
	{
		SelectionText->SetText(FText::FromString(SelectContent[CurSelection]));
		if(CurSelection == SelectContent.Num() - 1)
			RightButton->SetEnabled(false);
	}
		
	LeftButton->SetEnabled(true);
	ExecuteSelection.ExecuteIfBound(CurSelection);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

