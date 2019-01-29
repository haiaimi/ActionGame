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
	
public:
	AActionGameCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/**是否在释放技能*/
	UPROPERTY(BlueprintReadWrite)
	bool bInAbility;

	/**受攻击的反馈动画，为前、后、左、右的顺序*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> HitReactAnims;

	FTimerHandle YawTimerHandle;

	/**是否被Aurora减速*/
	UPROPERTY(BlueprintReadOnly)
	bool bFreezedSlow;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

public:
	/**普通攻击*/
	virtual void NormalAttack();

	/**下面是人物主要的三个技能*/
	virtual void Ability_Q();

	virtual void Ability_E();

	virtual void Ability_R();

protected:
	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	virtual EMoveDir::Type GetMoveDirection();

	/**玩家被冻结，减速*/
	UFUNCTION(BlueprintCallable)
	void ApplyFreezedParticle(class UParticleSystem* InParticle);

	/**玩家对受到的攻击进行反馈*/
	virtual bool HitReact(const FVector& HitPoint);
};

