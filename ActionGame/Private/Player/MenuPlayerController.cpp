// Fill out your copyright notice in the Description page of Project Settings.

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
	//UGameUserSettings::GetGameUserSettings()->SetResolutionScaleValueEx(20.f);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
	/*UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(3);
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);*/
	//HAIAIMIHelper::Debug_ScreenMessage(*GGameUserSettingsIni, 10.f);

	/*const TCHAR* Section = TEXT("ScalabilityGroups");

	if(GConfig)
	{
		HAIAIMIHelper::Debug_ScreenMessage(TEXT("GConfig"), 10.f);
		GConfig->SetInt(Section, TEXT("sg.AntiAliasingQuality"), 0, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);

		GConfig->SetInt(TEXT("haiaimi"), TEXT("one"), 0, GGameIni);
		GConfig->Flush(false, GGameIni);
	}*/
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
