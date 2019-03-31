// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionGameCharacter.h"
#include "ActionGameCharacter_Countess.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameCharacter_Countess : public AActionGameCharacter
{
	GENERATED_BODY()
	
public:
	AActionGameCharacter_Countess();

	/**������������Ҫ����������*/
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

	virtual void Ability_F()override;

	virtual bool HitReact(const FVector& HitPoint)override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	/**�ͷ�E���ܵ�����*/
	void SpawnRollingDarkSegemnts();

	/**���̵���*/
	void StabEnemy();

	/**
	 * Method:    GetEnemy 
	 * Param:     bool ClearPre       �Ƿ���Ҫ�����̬��ָ������ݣ��ڱ༭��������
	 * Returns:   class AActionGameCharacter*
	 */
	class AActionGameCharacter* GetEnemy(bool ClearPre = false);

	bool bFaceToEnemy;

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;

	virtual AActionGameCharacter* AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket = NAME_None)override;

private:
	UFUNCTION()
	void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnCountDownFinshed(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction);

	void TeleportArrive();

	/**Slip���ܷ���*/
	void SlipReturn();

	/**���R������Ч*/
	void ClearUltFX();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SwordCollision_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SwordCollision_R;

	///Q����
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* BeginTeleportEffect;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* TeleportCamEffect;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* TeleportReturnEffect;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* TeleportReturnCamEffect;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* TeleportShadowClone;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* TeleportShadowBurst;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* SlipTimerCountDown;

	class UParticleSystemComponent* CountDown;

	class UParticleSystemComponent* ShadowClone;

	///E����
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* RollingDarkSegment;

	/**�ܵ�E���ܹ�������Ч*/
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* RollingDarkImpact;

	///R����
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltEyeFX;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltEyeFX_Burst;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltAimingFX;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltImpactFX;

	///F����
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem*	SiphonHitFX;

	class UParticleSystemComponent* UltEye_Right;

	class UParticleSystemComponent* UltEye_Left;

	class UParticleSystemComponent* UltAiming;
};
