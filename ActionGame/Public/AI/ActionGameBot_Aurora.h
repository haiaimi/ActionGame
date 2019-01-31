// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionGameCharacter_Aurora.h"
#include "ActionGameType.h"
#include "ActionGameBot_Aurora.generated.h"

/**
 * AI����
 */
UCLASS()
class ACTIONGAME_API AActionGameBot_Aurora : public AActionGameCharacter_Aurora
{
	GENERATED_BODY()

public:
	AActionGameBot_Aurora();

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce /*=false*/)override;

	virtual EMoveDir::Type GetMoveDirection()override;

	virtual void SetMoveDir(EMoveDir::Type InDir)override;

public:
	/**��Ϊ��*/
	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BotBehavior;

	/**AI���ƶ����򣬾�������*/
	EMoveDir::Type AIMoveDir;
};
