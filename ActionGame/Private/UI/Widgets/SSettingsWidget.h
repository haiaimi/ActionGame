// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "SBaseMenuWidget.h"
#include "MenuHUD.h"
#include "MenuPlayerController.h"

/**
 * 
 */
class SSettingsWidget : public SBaseMenuWidget
{
	using FRenderTransformParam = TAttribute<TOptional<FSlateRenderTransform>>;

public:
	SLATE_BEGIN_ARGS(SSettingsWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwnerHUD)
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuPlayerController>, OwnerController)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void SetupAnimation();

	virtual void BackToPrevious()override;

	virtual void BackToShow()override;

	FORCEINLINE float GetCurAnimLerp() { return AnimHandle.GetLerp(); }

private:
	const struct FButtonStyle* ButtonStyle;

	const struct FButtonStyle* BackButtonStyle;

	const struct FButtonStyle* TagButtonStyle;

	const struct FButtonStyle* ApplySettingButtonStyle;

	FSlateBrush* BorderBackground;

	TArray<TSharedPtr<SButton>> SettingTagButtons;

	TArray<TSharedPtr<SBorder>> SettingBorders;

	TSharedPtr<SScrollBox> SettingList;
	
	TSharedPtr<STextBlock> SettingTitle;

	TSharedPtr<STextBlock> SettingDetails;

	FCurveSequence AnimSequence;

	FCurveHandle AnimHandle;

	/**ѡ����Ķ���*/
	TArray<FCurveHandle> SettingButtonHandles;

	TArray<FRenderTransformParam> ButtonTransformParams;

	TWeakObjectPtr<AMenuHUD> OwnerHUD;

	TWeakObjectPtr<class AMenuPlayerController> OwnerController;

	FIntPoint NewResolution;

	EWindowMode::Type NewFullscreenMode;

private:
	/**���ð�������*/
	void SetTagButtonHighlight(int32 ButtonIndex);

	void ShowGraphicSettingList();

	/**��ʾ���������б�*/
	void ShowOperationSettingList();

	void ShowCommonSetting();

	void ClearScrollBox();
};
