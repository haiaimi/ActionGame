// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAGPlayerController();

	virtual void BeginPlay()override;

	virtual void SetupInputComponent()override;

	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause())override;

	/**�����ӽ�*/
	void ConvertToDeathView();

	/**������Ϸ*/
	void EndGame();

	/**������������Ч��*/
	void ApplyGrayScreen();

	bool bIsGameEnd;

	bool bIsWon;

private:
	void PauseGame();

	UFUNCTION()
	void ToPlayerCamSmooth();

	UFUNCTION()
	void CameraOnDestroyed(AActor* DestroyedActor);

	/**�������̶���*/
	void SkipLevelSequence();
	
private:
	TSharedPtr<class SPauseMenuWidget> PauseWidget;

	UPROPERTY()
	ACameraActor* TempCameraActor;

	class ALevelSequenceActor* LevelSequence;
};
