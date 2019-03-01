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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/**���Ŀǰ�ƶ������״̬*/
	uint8 MoveDirStat;

	/**�������ֵ*/
	UPROPERTY(EditDefaultsOnly)
	float Health;

	/**���ܵ���ȴʱ��*/
	UPROPERTY(EditDefaultsOnly)
	TArray<float> SkillCoolingTimes;

	/**������ȴ��ʱ��*/
	TArray<FTimerHandle> SkillCoolingTimers;
	
public:
	AActionGameCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/**�Ƿ����ͷż���*/
	UPROPERTY(BlueprintReadWrite)
	bool bInAbility;

	/**�ܹ����ķ���������Ϊǰ�������ҵ�˳��*/
	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> HitReactAnims;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> CharacterMeshes;

	/**�Ƿ�Aurora����*/
	UPROPERTY(BlueprintReadOnly)
	bool bFreezedSlow;

	/**�Ƿ񱻶�ס*/
	bool bFreezedStop;

protected:
	virtual void BeginPlay()override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void MakeAbilityCooling(int32 Index);

	void SetAbilityReady(int32 Index);

public:
	/**��ͨ����*/
	virtual void NormalAttack();

	/**�����չ�*/
	virtual void ResetCombo() {};
	
	/**������������Ҫ����������*/
	virtual void Ability_Q();

	virtual void Ability_E();

	virtual void Ability_R();

	bool IsAbilityinCooling(int32 Index);

	float GetCoolingRate(int32 Index);

protected:
	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	virtual EMoveDir::Type GetMoveDirection();

	virtual void SetMoveDir(EMoveDir::Type InDir) {};

	///AuroraӢ�۶�Ӧ��Ч��
	/**��ұ����ᣬ����*/
	UFUNCTION(BlueprintCallable)
	void ApplyFreezedParticle(class UParticleSystem* InParticle);

	void ApplyFreezedCameraParticle(class UParticleSystem* InParticle);

	/**��Ҷ��ܵ��Ĺ������з���*/
	virtual bool HitReact(const FVector& HitPoint);

};

