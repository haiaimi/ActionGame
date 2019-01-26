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
	void ResetCombo();

	/**�������*/
	UFUNCTION(BlueprintCallable)
	void FreezeEnemy(class UParticleSystem* InParticle);

protected:
	virtual void Tick(float DeltaTime)override;

	/**��Ծ����*/
	virtual void Jump()override;

	virtual void MoveForward(float Value)override;

	virtual void MoveRight(float Value)override;

	virtual void NormalAttack()override;
	
	virtual void Ability_Q()override;

	virtual void Ability_E()override;

	virtual void Ability_R()override;

	void EmitFreeze();

	/**���ɱ���ƽ̨*/
	void SpawnIcePlatform();

	/**�����ɱ���ƽ̨ʱ���ȶ�ǰ���·�����м�⣬�������߼�¼����*/
	void DetectIceRoad();

protected:
	class USplineComponent* IceMoveSpline;

	UPROPERTY(VisibleAnywhere)
	bool SaveAttack;

	UPROPERTY(VisibleAnywhere)
	bool IsAttacking;

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

	/**Q���ܵ�����Ч��*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RushParticle;
	
	/**R���ܵ�����*/
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* RAbilityParticle;

	FTransform FreezeTransform;

	UPROPERTY(BlueprintReadWrite)
	bool bTurboJumpAccelerate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> IcePlatform;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* DecalMaterial;

	class AActor* PreIcePlatform;

	float IcePlatformOffset;
};
