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

	/**AI释放技能，按照一定优先级释放*/
	UFUNCTION(BlueprintCallable)
	void AICastAbility();

	void ResetSurroundState();

	void ResetSurroundCallable();

	/**行为树*/
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;

protected:
	//仅限于AI中使用
	TWeakObjectPtr<AActionGameCharacter>& GetEnemy();

	TWeakObjectPtr<class AActionAIController> OwnerController;

	/**横向方向*/
	EMoveDir::Type HoriDir;

	/**纵向方向*/
	EMoveDir::Type VertDir;
	
	/**AI围绕玩家状态*/
	bool bInSurround;

	/**控制Ai是否可以环绕，AI不可能一直都能进入环绕状态*/
	bool bCanSurround;
};
