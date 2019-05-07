// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionGameCharacter_Countess.h"
#include "GameBotInterface.h"
#include "ActionGameBot_Countess.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameBot_Countess : public AActionGameCharacter_Countess, public IGameBotInterface
{
	GENERATED_BODY()

public:
	AActionGameBot_Countess();

	virtual void PostInitializeComponents()override;

	virtual void Tick(float DeltaTime)override;

	///IGameBotInterface
	virtual void SetAIRotation(FRotator Rotator)override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	virtual EMoveDir::Type GetMoveDirection()override;

	/**��Ϊ��*/
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;

protected:
	TWeakObjectPtr<AActionGameCharacter>& GetEnemy();

	TWeakObjectPtr<class AActionAIController> OwnerController;

	/**������*/
	EMoveDir::Type HoriDir;

	/**������*/
	EMoveDir::Type VertDir;
	
	/**AIΧ�����״̬*/
	bool bInSurround;
};
