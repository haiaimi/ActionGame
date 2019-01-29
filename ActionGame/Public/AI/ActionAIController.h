// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ActionAIController.generated.h"

/**
 * AI敌人控制器
 */
UCLASS()
class ACTIONGAME_API AActionAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

public:
	AActionAIController();

	// Begin AController interface
	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;
	virtual void BeginInactiveState() override;
	// End APlayerController interface
	
	class AActionGameCharacter* GetEnemy();

	void SetAIFreezedValue();

private:
	int32 EnemyKeyID;

	int32 FreezedKeyID;
};
