// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMenuHUD();

	virtual void DrawHUD()override;
	
private:
	TSharedPtr<class SMainMenuWidget> MainMenu;
};
