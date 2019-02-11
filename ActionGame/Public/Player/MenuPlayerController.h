// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMenuPlayerController();

	virtual void BeginPlay()override;

	virtual void SetupInputComponent() override;

	void BackToPreMenu();

	void SetCurWidget(TSharedPtr<class SBaseMenuWidget> InWidget) { CurWidget = InWidget; }

	void BeginMouseClicked();

	void EndMouseClicked();

private:
	TSharedPtr<class SBaseMenuWidget> CurWidget;

	class AHeroDetailPlatform* CurDetailPlatform;
};
