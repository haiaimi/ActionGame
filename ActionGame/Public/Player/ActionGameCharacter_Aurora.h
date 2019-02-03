// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameCharacter.h"
#include "ActionGameCharacter_Aurora.generated.h"

/**
 * Aurora人物对应的类
 */
UCLASS()
class ACTIONGAME_API AActionGameCharacter_Aurora : public AActionGameCharacter
{
	GENERATED_BODY()

public:
	AActionGameCharacter_Aurora();

	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();

	UFUNCTION(BlueprintCallable)
	virtual void ResetCombo()override;

	/**冻结敌人*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy();

	void FreezeEnemyImpl(class AActor* Enemy);

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	void AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor);

public:
	/**跳跃动作*/
	virtual void Jump()override;

	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;

	virtual void NormalAttack()override;
	
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

	virtual bool HitReact(const FVector& HitPoint)override;

	UFUNCTION(BlueprintCallable)
	bool IsCastAbility_R();

	void EmitFreeze();

	/**生成冰块平台*/
	void SpawnIcePlatform();

	/**在生成冰块平台时首先对前面的路况进行检测，用样条线记录下来*/
	void DetectIceRoad();

	UFUNCTION()
	void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnQAbilityBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void PlayerCanAttack() { bCanAttack = true; }

	UFUNCTION(BlueprintCallable)
	void EffectOfAbilityE(class AActor* Enemy);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SwordCollsion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* QAbilityCollision;

	class USplineComponent* IceMoveSpline;

	UPROPERTY(VisibleAnywhere)
	bool SaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAttacking;

	UPROPERTY(BlueprintReadWrite)
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere)
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

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* FreezeSegment;
	
	/**R技能的粒子*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RAbilityParticle;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* Freezed_Slow;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* Freezed_Stop;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ImpactParticle;

	FTransform FreezeTransform;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> IcePlatform;

	/**用于E技能的范围检测*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EAbilityDetection;

	class AActor* PreIcePlatform;

	float IcePlatformOffset;

	float MoveTime;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bTurboJumpAccelerate;
};
