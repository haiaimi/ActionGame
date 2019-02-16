// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SSelectBoxWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FExecuteSelection, float)

	SLATE_BEGIN_ARGS(SSelectBoxWidget)
	{}
	SLATE_ARGUMENT(FText, SelectName)
	SLATE_ARGUMENT(TArray<FString>, SelectContent)
	SLATE_ARGUMENT(int32, CurSelection)
	SLATE_EVENT(FExecuteSelection, ExecuteSelection)
	SLATE_EVENT(FSimpleDelegate, SelectionOnHovered)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetDelegate(FExecuteSelection InDelegate);

	void ToBack();

	void ToForward();
private:
	const struct FButtonStyle* SelectBoxButtonStyle;

	const struct FButtonStyle* BackwardButtonStyle;

	const struct FButtonStyle* ForwardButtonStyle;

	TSharedPtr<STextBlock> SelectionName;

	TSharedPtr<STextBlock> SelectionText;

	TSharedPtr<SButton> LeftButton;

	TSharedPtr<SButton> RightButton;

	TArray<FString> SelectContent;

	int32 CurSelection;

	FExecuteSelection ExecuteSelection;

	FSimpleDelegate SelectionOnHovered;
};
