// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACTIONGAME_API AGWidgetInterface
{
public:
	virtual void BackToPrevious() = 0;

	virtual ~AGWidgetInterface() = default;
};
