// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameBot_Countess.h"
#include "ActionAIController.h"
#include "HAIAIMIHelper.h"

AActionGameBot_Countess::AActionGameBot_Countess():
	HoriDir(EMoveDir::Right),
	VertDir(EMoveDir::Forward),
	bInSurround(true)
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

	TWeakObjectPtr<AActionGameCharacter>& Enemy = GetEnemy();     //»ñÈ¡µÐÈË
	if (bInSurround && Enemy.IsValid())
	{
		const float Distance = (Enemy->GetActorLocation() - GetActorLocation()).Size2D();

		if (Distance < 600.f)
			MoveForward(-1.f);
		if (Distance > 800.f)
			MoveForward(1.f);
		
		//MoveRight(1.f);

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

EMoveDir::Type AActionGameBot_Countess::GetMoveDirection()
{
	//HAIAIMIHelper::Debug_ScreenMessage(TEXT("Compile Test"));
	return Super::GetMoveDirection();
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
