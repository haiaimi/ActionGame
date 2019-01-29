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
	void ResetCombo();

	/**冻结敌人*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy();

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

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

	void EmitFreeze();

	/**生成冰块平台*/
	void SpawnIcePlatform();

	/**在生成冰块平台时首先对前面的路况进行检测，用样条线记录下来*/
	void DetectIceRoad();

	UFUNCTION()
	void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void PlayerCanAttack() { bCanAttack = true; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SwordCollsion;

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

	/**Q技能的粒子效果*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RushParticle;
	
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

	class AActor* PreIcePlatform;

	float IcePlatformOffset;

	float MoveTime;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bTurboJumpAccelerate;
};
