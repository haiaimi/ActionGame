// Fill out your copyright notice in the Description page of Project Settings.

#include "AGPlayerController.h"
#include "GameWidgets/SPauseMenuWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "HAIAIMIHelper.h"
#include "GameHUD.h"

AAGPlayerController::AAGPlayerController()
{

}

void AAGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AAGPlayerController::PauseGame);
	}
}

bool AAGPlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate)
{
	bShowMouseCursor = bPause;
	SetInputMode(FInputModeGameOnly());

	AGameHUD* MHUD = Cast<AGameHUD>(GetHUD());
	if (MHUD && MHUD->GetHUDWidget().IsValid())
	{
		MHUD->GetHUDWidget()->ReverseAnim();
	}

	if (bPause)
	{
		if (PauseWidget.IsValid())
			PauseWidget->SetVisibility(EVisibility::Visible);
		else
		{
			SAssignNew(PauseWidget, SPauseMenuWidget)
				.OwnerController(this);

			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget)
				.PossiblyNullContent(PauseWidget.ToSharedRef()),
				1
			);
		}
	}
	else
		if (PauseWidget.IsValid())PauseWidget->SetVisibility(EVisibility::Hidden);

	return Super::SetPause(bPause, CanUnpauseDelegate);
}

void AAGPlayerController::PauseGame()
{
	SetPause(true);
}
