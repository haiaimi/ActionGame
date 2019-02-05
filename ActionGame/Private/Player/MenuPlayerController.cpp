// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"

AMenuPlayerController::AMenuPlayerController()
{
	bShowMouseCursor = true;
}

void AMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());

}
