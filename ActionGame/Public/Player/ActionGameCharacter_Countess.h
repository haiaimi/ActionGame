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

	/**下面是人物主要的三个技能*/
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

private:
	UFUNCTION()
	void OnCountDownFinshed(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction);

	/**Slip技能返回*/
	void SlipReturn();

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
};
