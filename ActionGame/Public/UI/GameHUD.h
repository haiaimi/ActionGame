// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameWidgets/SHUDWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD()override;

	TSharedPtr<class SHUDWidget> GetHUDWidget() { return HUDWidget; }

private:
	TSharedPtr<class SHUDWidget> HUDWidget;
};
