// Fill out your copyright notice in the Description page of Project Settings.

#include "AGPlayerController.h"
#include "GameWidgets/SPauseMenuWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "HAIAIMIHelper.h"
#include "GameHUD.h"
#include "UObjectIterator.h"
#include "EngineUtils.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "CinematicCamera/Public/CineCameraActor.h"

AAGPlayerController::AAGPlayerController()
{
	
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ALevelSequenceActor> It(GetWorld()); It; ++It)
	{
		if ((*It)->SequencePlayer)
		{
			(*It)->SequencePlayer->OnStop.AddDynamic(this, &AAGPlayerController::ToPlayerCamSmooth);
			break;
		}
	}

	for (TActorIterator<ACineCameraActor> It(GetWorld()); It; ++It)
	{
		(*It)->OnDestroyed.AddDynamic(this, &AAGPlayerController::CameraOnDestroyed);
		break;
	}
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

void AAGPlayerController::ConvertToDeathView()
{
	if (GetPawn() && TempCameraActor)
	{
		TempCameraActor->SetActorLocation(GetPawn()->GetActorLocation() + FVector(0.f, 0.f, 600.f));
		TempCameraActor->SetActorRotation(FRotator(-90.f, GetPawn()->GetActorRotation().Yaw, 0.f));
		this->SetViewTargetWithBlend(TempCameraActor, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
	}
}

void AAGPlayerController::PauseGame()
{
	SetPause(true);
}

void AAGPlayerController::ToPlayerCamSmooth()
{
	this->SetViewTargetWithBlend(TempCameraActor, 0.f, EViewTargetBlendFunction::VTBlend_Cubic);
	this->SetViewTargetWithBlend(GetPawn(), 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
}

void AAGPlayerController::CameraOnDestroyed(AActor* DestroyedActor)
{
	TempCameraActor = GetWorld()->SpawnActor<ACameraActor>(DestroyedActor->GetActorLocation(), DestroyedActor->GetActorRotation());
}
