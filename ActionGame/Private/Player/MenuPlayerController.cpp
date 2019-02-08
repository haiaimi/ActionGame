// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "GameFramework/Pawn.h"
#include "HAIAIMIHelper.h"
#include "UI/SBaseMenuWidget.h"

AMenuPlayerController::AMenuPlayerController()
{
	bShowMouseCursor = true;
}

void AMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());

	GetPawn()->SetActorHiddenInGame(true);
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
	HAIAIMIHelper::Debug_ScreenMessage(TEXT("Back Menu"));
	if(CurWidget.IsValid())
	{
		CurWidget->BackToPrevious();
	}
}
