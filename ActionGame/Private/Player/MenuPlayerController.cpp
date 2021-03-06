﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"
#include "GameFramework/Pawn.h"
#include "HAIAIMIHelper.h"
#include "UI/Widgets/SBaseMenuWidget.h"
#include "UI/MenuHUD.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/LocalPlayer.h"
#include "GameActors/HeroDetailPlatform.h"
#include "GameFramework/GameUserSettings.h"
#include "CoreGlobals.h"

//const TArray<FString> AbilityMappingNames = { TEXT("Ability_Q"), TEXT("Ability_E"), TEXT("Ability_R"), TEXT("Ability_F") };

AMenuPlayerController::AMenuPlayerController():
	CurDetailPlatform(nullptr)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	GetLocalPlayer()->ViewportClient->SetCaptureMouseOnClick(EMouseCaptureMode::CaptureDuringMouseDown);    //鼠标按下时就监听事件，不然需要双击
	GetLocalPlayer()->ViewportClient->SetMouseLockMode(EMouseLockMode::LockAlways);
	GetPawn()->SetActorHiddenInGame(true);

	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode());
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
	FString Ref;
	GConfig->GetString(TEXT("haiaimi"), TEXT("Language"), Ref, GGameIni);
	HAIAIMIHelper::ChangeLocalization(Ref);
}

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("DetectPlatform"), EKeys::LeftMouseButton));

	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("BackToPreMenu", EInputEvent::IE_Pressed, this, &AMenuPlayerController::BackToPreMenu);
		InputComponent->BindAction(TEXT("DetectPlatform"), EInputEvent::IE_Pressed, this, &AMenuPlayerController::BeginMouseClicked);
		InputComponent->BindAction(TEXT("DetectPlatform"), EInputEvent::IE_Released, this, &AMenuPlayerController::EndMouseClicked);
	}
}

void AMenuPlayerController::BackToPreMenu()
{
	if(CurWidget.IsValid())
	{
		CurWidget->BackToPrevious();
	}
}

void AMenuPlayerController::BeginMouseClicked()
{
	FHitResult Result;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Result);
	if (Result.GetActor())
	{
		if (AHeroDetailPlatform* DetailPlatform = Cast<AHeroDetailPlatform>(Result.GetActor()))
		{
			CurDetailPlatform = DetailPlatform;
			FVector2D MousePos;
			GetMousePosition(MousePos.X, MousePos.Y);
			DetailPlatform->StartRotate(MousePos);
		}
	}
}

void AMenuPlayerController::EndMouseClicked()
{
	if (CurDetailPlatform)
		CurDetailPlatform->EndRotate();
	CurDetailPlatform = nullptr;
}
