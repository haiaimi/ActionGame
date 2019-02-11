// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "GameFramework/Pawn.h"
#include "HAIAIMIHelper.h"
#include "UI/Widgets/SBaseMenuWidget.h"
#include "UI/MenuHUD.h"

AMenuPlayerController::AMenuPlayerController()
{
	bShowMouseCursor = true;
}

void AMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	GetPawn()->SetActorHiddenInGame(true);
	ConsoleCommand("r.SetRes 1920x1080");
}

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("BackToPreMenu", EInputEvent::IE_Pressed, this, &AMenuPlayerController::BackToPreMenu);
	}
}

void AMenuPlayerController::BackToPreMenu()
{
	if(CurWidget.IsValid())
	{
		CurWidget->BackToPrevious();
	}
}
