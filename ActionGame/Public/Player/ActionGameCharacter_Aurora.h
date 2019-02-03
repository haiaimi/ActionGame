// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameCharacter.h"
#include "ActionGameCharacter_Aurora.generated.h"

/**
 * Aurora�����Ӧ����
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

	/**�������*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy();

	void FreezeEnemyImpl(class AActor* Enemy);

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	void AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor);

public:
	/**��Ծ����*/
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

	/**���ɱ���ƽ̨*/
	void SpawnIcePlatform();

	/**�����ɱ���ƽ̨ʱ���ȶ�ǰ���·�����м�⣬�������߼�¼����*/
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

	/**��ͨ�����Ķ���*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> NormalAttackAnims;

	/**���ܶ���*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> AbilityAnims;

	/**��ǿ����Ծ����*/
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* TurboJumpAnim;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* FreezeSegment;
	
	/**R���ܵ�����*/
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

	/**����E���ܵķ�Χ���*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EAbilityDetection;

	class AActor* PreIcePlatform;

	float IcePlatformOffset;

	float MoveTime;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bTurboJumpAccelerate;
};
