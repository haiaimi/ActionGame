// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/MenuGameMode.h"
#include "UI/MenuHUD.h"
#include "MenuPlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	HUDClass = AMenuHUD::StaticClass();
	PlayerControllerClass=AMenuPlayerController::StaticClass();
}
