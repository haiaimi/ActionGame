// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SInputBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInputBoxWidget)
	{}
	SLATE_ARGUMENT(FText, SelectName)
	SLATE_ARGUMENT(FText, CurSelection)
	//SLATE_EVENT(FExecuteSelection, ExecuteSelection)
	SLATE_EVENT(FSimpleDelegate, SelectionOnHovered)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	const struct FButtonStyle* SelectBoxButtonStyle;

	TSharedPtr<STextBlock> SelectionName;

	TSharedPtr<STextBlock> SelectionText;

	FSimpleDelegate SelectionOnHovered;
};
