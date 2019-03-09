// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "../Styles/UIAssetWidgetStyle.h"
#include "ActionGameCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"

/**
 *  人物血条控件
 */
class SHealthBarWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHealthBarWidget):
		_Owner(nullptr),
		_BarPos(EHorizontalAlignment::HAlign_Left),
		_HeroIndex(0)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AActionGameCharacter>, Owner)
	SLATE_ARGUMENT(EHorizontalAlignment, BarPos)
	SLATE_ARGUMENT(int32, HeroIndex)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetHealthBar(float NewHealth);

private:
	const struct FUIAssetStyle* UIStyle;

	TSharedPtr<SImage> HeroHead;

	TSharedPtr<SImage> HealthBar;

	TWeakObjectPtr<class AActionGameCharacter> Owner;

	EHorizontalAlignment BarPos;

	class UMaterialInstanceDynamic* HealthBarMaterialDInstance;
};
