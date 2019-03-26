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

	/**�ͷ�E���ܵ�����*/
	void SpawnRollingDarkSegemnts();

	/**���̵���*/
	void StabEnemy();

	bool bFaceToEnemy;

	class AActionGameCharacter* Enemy;

protected:
	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;

	virtual void Tick(float DeltaTime)override;

private:
	UFUNCTION()
	void OnCountDownFinshed(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction);

	/**Slip���ܷ���*/
	void SlipReturn();

	/**���R������Ч*/
	void ClearUltFX();

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

	///R����
	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltEyeFX;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltEyeFX_Burst;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityEffect")
	class UParticleSystem* UltAimingFX;

	class UParticleSystemComponent* UltEye_Right;

	class UParticleSystemComponent* UltEye_Left;

	class UParticleSystemComponent* UltAiming;
};
