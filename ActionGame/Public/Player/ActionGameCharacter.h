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
	
public:
	AActionGameCharacter();

	DECLARE_DELEGATE_OneParam(FOnHealthChanged, float);

	/**普通攻击*/
	virtual void NormalAttack();

	void ComboAttackSave();

	/**重置普攻*/
	virtual void ResetCombo();
	
	/**下面是人物主要的三个技能*/
	virtual void Ability_Q();

	virtual void Ability_E();

	virtual void Ability_R();

	bool IsAbilityinCooling(EAbilityType::Type AbilityType);

	float GetCoolingRate(EAbilityType::Type AbilityType);

	AActionGameCharacter* GetAIEnemy();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	virtual EMoveDir::Type GetMoveDirection();

	virtual void SetMoveDir(EMoveDir::Type InDir) {};

	///Aurora英雄对应的效果
	/**玩家被冻结，减速*/
	UFUNCTION(BlueprintCallable)
	void ApplyFreezedParticle(class UParticleSystem* InParticle);

	void ApplyFreezedCameraParticle(class UParticleSystem* InParticle);

	void EndFreezedSlow();

	void EndFreezedStop();

	/**玩家对受到的攻击进行反馈*/
	virtual bool HitReact(const FVector& HitPoint);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool IsDead() { return bDead; }

	UPROPERTY(EditAnywhere)
	bool bCheatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**是否在释放技能*/
	bool bInAbility;

	bool bTurboJumpAccelerate;

	/**受攻击的反馈动画，为前、后、左、右的顺序*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> HitReactAnims;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> CharacterMeshes;

	/**是否被Aurora减速*/
	UPROPERTY(BlueprintReadOnly)
	bool bFreezedSlow;

	/**是否被冻住*/
	bool bFreezedStop;

	float YawSpeed;

	float PitchSpeed;

	FOnHealthChanged OnHealthChanged;

protected:
	virtual void BeginPlay()override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	virtual void AddControllerYawInput(float Val)override;

	virtual void AddControllerPitchInput(float Val)override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void MakeAbilityCooling(EAbilityType::Type AbilityType);

	void SetAbilityReady(int32 Index);

	bool SaveAttack;

	bool IsAttacking;

	bool bCanAttack;

	int32 AttackCount;

	/**普通攻击的动画*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> NormalAttackAnims;

	/**技能动画*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> AbilityAnims;

	/**加强版跳跃动画*/
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* TurboJumpAnim;

	/**技能的冷却时间*/
	UPROPERTY(EditDefaultsOnly)
	TArray<float> SkillCoolingTimes;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/**玩家目前移动方向的状态*/
	uint8 MoveDirStat;

	/**玩家生命值*/
	UPROPERTY(EditDefaultsOnly)
	float Health;

	/**技能冷却定时器*/
	TArray<FTimerHandle> SkillCoolingTimers;

	/**玩家是否已经死亡*/
	bool bDead;
};

