// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SBaseMenuWidget : public SCompoundWidget
{
public:
	virtual void BackToPrevious() {};

	TWeakPtr<SCompoundWidget> PreWidget;
};
