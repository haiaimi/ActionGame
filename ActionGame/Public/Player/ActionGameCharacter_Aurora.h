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

	/**�������*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy();

	void FreezeEnemyImpl(class AActor* Enemy);

public:

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	AActionGameCharacter* AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket = NAME_None)override;

public:
	/**��Ծ����*/
	virtual void Jump()override;

	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;
	
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

	virtual void Ability_F()override;

	virtual bool HitReact(const FVector& HitPoint)override;

	void EmitFreeze();

	/**���ɱ���ƽ̨*/
	void SpawnIcePlatform();

	/**�����ɱ���ƽ̨ʱ���ȶ�ǰ���·�����м�⣬�������߼�¼����*/
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
	
	/**R���ܵ�����*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RAbilityParticle;

	/**F���ܶ���������AuroraF�����ж������*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> FAbilityAnims;
	
	/**�Ե��˵���Ч*/
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

	/**����E���ܵķ�Χ���*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EAbilityDetection;

	class AActor* PreIcePlatform;

	float IcePlatformOffset;

	float MoveTime;

	/**Q���ܵ�һ�ι���ʱ��*/
	float QFirstAttackTime;

	/**����F���ܵ���Ծ*/
	uint8 AbilityJumpDir;
};
