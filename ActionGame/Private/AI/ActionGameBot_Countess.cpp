// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameBot_Countess.h"
#include "ActionAIController.h"
#include "HAIAIMIHelper.h"
#include "TimerManager.h"
#include "Engine/World.h"

AActionGameBot_Countess::AActionGameBot_Countess():
	OwnerController(nullptr),
	HoriDir(EMoveDir::Right),
	VertDir(EMoveDir::Forward),
	bInSurround(true),
	bCanSurround(true)
{
	AIControllerClass = AActionAIController::StaticClass();
	bUseControllerRotationYaw = true;
}

void AActionGameBot_Countess::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BehaviorTree = BotBehavior;
}

void AActionGameBot_Countess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GetController<AActionAIController>()->HasStarted())
		MoveRight((HoriDir == EMoveDir::Right ? 1.f : -1.f));*/

	TWeakObjectPtr<AActionGameCharacter>& Enemy = GetEnemy();     //获取敌人
	
	if (OwnerController.IsValid())
		bInSurround = OwnerController->GetSurroundState();
	else
	{
		OwnerController = GetController<AActionAIController>();
		if (!OwnerController.IsValid())
			return;
	}
	
	if (bInSurround && Enemy.IsValid())
	{
		const float Distance = (Enemy->GetActorLocation() - GetActorLocation()).Size2D();
		FRotator AIRot = GetControlRotation();
		AIRot.Roll = 0.f;
		SetActorRotation(AIRot);

		if (Distance < 600.f)
			MoveForward(-1.f);
		if (Distance > 800.f)
			MoveForward(1.f);

		bUseControllerRotationYaw = true;
	}
}

void AActionGameBot_Countess::SetAIRotation(FRotator Rotator)
{
	
}

void AActionGameBot_Countess::FaceRotation(FRotator NewRotation, float DeltaTime /*= 0.f*/)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 10.0f);

	ACharacter::FaceRotation(CurrentRotation, DeltaTime);
}

float AActionGameBot_Countess::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float CurHealth = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (bCanSurround && !bInSurround && GetRemainHealthRate() <= 0.4f)
	{
		bCanSurround = false;
		if (OwnerController.IsValid())
			OwnerController->SetSurroundState(true);

		FTimerHandle TimerHandle, TimerHandle1;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AActionGameBot_Countess::ResetSurroundState, 5.f, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &AActionGameBot_Countess::ResetSurroundCallable, 15.f, false);
	}

	return CurHealth;
}

EMoveDir::Type AActionGameBot_Countess::GetMoveDirection()
{
	if (bInSurround)
		return Super::GetMoveDirection();
	else
		return EMoveDir::Forward;
}

void AActionGameBot_Countess::AICastAbility()
{
	TWeakObjectPtr<AActionGameCharacter>& Enemy = GetEnemy();
	if (!Enemy.IsValid() || bFreezedStop)return;    //AI被冻住同样不能释放技能
	const float Distance = (Enemy->GetActorLocation() - GetActorLocation()).Size2D();

	if (Distance < 1000.f && Distance > 200.f)
	{
		if (!IsAbilityinCooling(EAbilityType::QAbility))
		{
			Ability_Q();   //释放Q技能
			return;
		}
		else
		{
			const float RemainHealthRate = GetRemainHealthRate();
			const FVector ShadowPos = GetShadowPosition();
			if (ShadowPos != FVector::ZeroVector)
			{
				const bool bEnemyNearShadow = (ShadowPos - Enemy->GetActorLocation()).Size2D() < (GetActorLocation() - Enemy->GetActorLocation()).Size2D();
				if (bEnemyNearShadow && RemainHealthRate > 0.4f)
					Ability_Q();
				if (!bEnemyNearShadow && RemainHealthRate <= 0.4f)
					Ability_Q();
			}
		}
	}

	if (!IsAbilityinCooling(EAbilityType::RAbility) && Distance < 150.f)
	{
		Ability_R();
		return;
	}

	if (!IsAbilityinCooling(EAbilityType::EAbility) && Distance < 600.f)
	{
		Ability_E();
		return;
	}


	if (!IsAbilityinCooling(EAbilityType::FAbility) && Distance < 600.f)
	{
		Ability_F();
		return;
	}
}

void AActionGameBot_Countess::ResetSurroundState()
{
	if (OwnerController.IsValid())
			OwnerController->SetSurroundState(false);
}

void AActionGameBot_Countess::ResetSurroundCallable()
{
	bCanSurround = true;
}

TWeakObjectPtr<AActionGameCharacter>& AActionGameBot_Countess::GetEnemy()
{
	static TWeakObjectPtr<AActionGameCharacter> Enemy = nullptr;

	if (!Enemy.IsValid())
	{
		if (AActionAIController* AIC = GetController<AActionAIController>())
			Enemy = AIC->GetEnemy();
	}

	return Enemy;
}
