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
	virtual void TickActor( float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction ) override;

	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginInactiveState() override;
	// End AController interface
	
	class AActionGameCharacter* GetEnemy();

	void SetAIFreezedValue();

	UFUNCTION(BlueprintCallable)
	bool ShouldMoveBack();

	bool IsMovingBack();

	/**游戏是否已经开始*/
	bool HasStarted();

	void StartAIPlayer();
private:
	
	int32 EnemyKeyID;

	int32 FreezedKeyID;

	int32 MoveBackKeyID;

	class AActionGameCharacter* Enemy;
};
