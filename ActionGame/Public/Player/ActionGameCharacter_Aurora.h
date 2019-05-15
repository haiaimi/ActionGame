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

	/**冻结敌人*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy();

	void FreezeEnemyImpl(class AActor* Enemy);

public:

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	AActionGameCharacter* AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket = NAME_None)override;

public:
	/**跳跃动作*/
	virtual void Jump()override;

	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;
	
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

	virtual void Ability_F()override;

	virtual bool HitReact(const FVector& HitPoint)override;

	void EmitFreeze();

	/**生成冰块平台*/
	void SpawnIcePlatform();

	/**在生成冰块平台时首先对前面的路况进行检测，用样条线记录下来*/
	void DetectIceRoad();

	UFUNCTION()
	void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnQAbilityBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void DisableQDetection();

	void PlayerCanAttack() { bCanAttack = true; }

	UFUNCTION(BlueprintCallable)
	void EffectOfAbilityE(class AActor* Enemy);

	bool bInAbilityJump;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SwordCollsion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* QAbilityCollision;

	class USplineComponent* IceMoveSpline;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* FreezeSegment;
	
	/**R技能的粒子*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RAbilityParticle;

	/**F技能动画，由于AuroraF技能有多个动画*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> FAbilityAnims;
	
	/**对敌人的特效*/
	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* Freezed_Slow;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* Freezed_Stop;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* CamFrostParticle_Slowed;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* CamFrostParticle_Frozen;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* ExplodeParticle_Frozen;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyEffect")
	class UParticleSystem* ExplodeParticle_Slow;
		
	FTransform FreezeTransform;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> IcePlatform;

	/**用于E技能的范围检测*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EAbilityDetection;

	class AActor* PreIcePlatform;

	float IcePlatformOffset;

	float MoveTime;

	/**Q技能第一次攻击时间*/
	float QFirstAttackTime;

	/**用于F技能的跳跃*/
	uint8 AbilityJumpDir;
};
