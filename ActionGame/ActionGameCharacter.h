// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionGameType.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/**玩家目前移动方向的状态*/
	uint8 MoveDirStat;

	bool bCanUpdateControlYaw;
	
public:
	AActionGameCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/**是否在释放技能*/
	UPROPERTY(BlueprintReadWrite)
	bool bInAbility;

	FTimerHandle YawTimerHandle;

	/**是否被Aurora减速*/
	bool bFreezedSlow;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	/**普通攻击*/
	virtual void NormalAttack();

	/**下面是人物主要的三个技能*/
	virtual void Ability_Q();

	virtual void Ability_E();

	virtual void Ability_R();

protected:
	


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	EMoveDir::Type GetMoveDirection();


	/**玩家被冻结，减速*/
	UFUNCTION(BlueprintCallable)
	void ApplyFreezedParticle(class UParticleSystem* InParticle);
};

