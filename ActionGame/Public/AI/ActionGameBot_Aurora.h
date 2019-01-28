// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionGameCharacter_Aurora.h"
#include "ActionGameBot_Aurora.generated.h"

/**
 * AIµÐÈË
 */
UCLASS()
class ACTIONGAME_API AActionGameBot_Aurora : public AActionGameCharacter_Aurora
{
	GENERATED_BODY()

public:
	AActionGameBot_Aurora();


	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

public:
	/**ÐÐÎªÊ÷*/
	UPROPERTY(EditAnywhere, Category=Behavior)
	class UBehaviorTree* BotBehavior;
	
};
