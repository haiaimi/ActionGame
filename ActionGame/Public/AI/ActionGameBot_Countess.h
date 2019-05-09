// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionGameCharacter_Countess.h"
#include "GameBotInterface.h"
#include "ActionGameBot_Countess.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameBot_Countess : public AActionGameCharacter_Countess, public IGameBotInterface
{
	GENERATED_BODY()

public:
	AActionGameBot_Countess();

	virtual void PostInitializeComponents()override;

	virtual void Tick(float DeltaTime)override;

	///IGameBotInterface
	virtual void SetAIRotation(FRotator Rotator)override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual EMoveDir::Type GetMoveDirection()override;

	/**AI�ͷż��ܣ�����һ�����ȼ��ͷ�*/
	UFUNCTION(BlueprintCallable)
	void AICastAbility();

	void ResetSurroundState();

	void ResetSurroundCallable();

	/**��Ϊ��*/
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;

protected:
	//������AI��ʹ��
	TWeakObjectPtr<AActionGameCharacter>& GetEnemy();

	TWeakObjectPtr<class AActionAIController> OwnerController;

	/**������*/
	EMoveDir::Type HoriDir;

	/**������*/
	EMoveDir::Type VertDir;
	
	/**AIΧ�����״̬*/
	bool bInSurround;

	/**����Ai�Ƿ���Ի��ƣ�AI������һֱ���ܽ��뻷��״̬*/
	bool bCanSurround;
};
