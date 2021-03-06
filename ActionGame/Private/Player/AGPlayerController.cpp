// Fill out your copyright notice in the Description page of Project Settings.

#include "AGPlayerController.h"
#include "SPauseMenuWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "HAIAIMIHelper.h"
#include "GameHUD.h"
#include "UObjectIterator.h"
#include "EngineUtils.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "CinematicCamera/Public/CineCameraActor.h"
#include "ActionAIController.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerInput.h"

const TArray<FString> AbilityMappingNames = { TEXT("Ability_Q"), TEXT("Ability_E"), TEXT("Ability_R"), TEXT("Ability_F") };
const TArray<FString> DefaultAbilityKeyNames = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("LeftShift") };

AAGPlayerController::AAGPlayerController():
	bIsGameEnd(false),
	bIsWon(false),
	LevelSequence(nullptr)
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
			LevelSequence = (*It);
			break;
		}
	}

	for (TActorIterator<ACineCameraActor> It(GetWorld()); It; ++It)
	{
		(*It)->OnDestroyed.AddDynamic(this, &AAGPlayerController::CameraOnDestroyed);
		break;
	}

	FString Ref;
	for (int32 i = 0; i < AbilityMappingNames.Num(); ++i)
	{
		GConfig->GetString(TEXT("haiaimi"), *AbilityMappingNames[i], Ref, GGameIni);
		PlayerInput->AddActionMapping(FInputActionKeyMapping(*AbilityMappingNames[i], Ref.IsEmpty() ? FKey(*DefaultAbilityKeyNames[i]) : FKey(*Ref)));
	}
}

void AAGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AAGPlayerController::PauseGame);
		InputComponent->BindAction(TEXT("SkipSequence"), EInputEvent::IE_Pressed, this, &AAGPlayerController::SkipLevelSequence);
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

void AAGPlayerController::EndGame()
{
	bIsGameEnd = true;
	if (GetPawn())GetPawn()->DisableInput(this);
	PauseGame();
}

void AAGPlayerController::ApplyGrayScreen()
{
	UMaterialInterface* ScreenGray = LoadObject<UMaterialInterface>(this, TEXT("/Game/CustomEffect/ScreenGray"));
	if (ScreenGray && TempCameraActor)
		TempCameraActor->GetCameraComponent()->PostProcessSettings.AddBlendable(ScreenGray, 1.f);
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

void AAGPlayerController::SkipLevelSequence()
{
	if (LevelSequence && LevelSequence->SequencePlayer->IsPlaying())
	{
		LevelSequence->SequencePlayer->GoToEndAndStop();
		Cast<AActionGameCharacter>(GetPawn())->GetAIEnemy()->GetController<AActionAIController>()->StartAIPlayer();
	}

	//const TArray<FPostProcessSettings>* PostProcess;
	//const TArray<float>* Blends;
	//this->PlayerCameraManager->GetCachedPostProcessBlends(PostProcess, Blends);
}
