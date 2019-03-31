// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameBot_Countess.h"
#include "ActionAIController.h"
#include "HAIAIMIHelper.h"

AActionGameBot_Countess::AActionGameBot_Countess():
	VertDir(EMoveDir::Forward),
	HoriDir(EMoveDir::Right)
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

	if (GetController<AActionAIController>()->HasStarted())
		MoveRight((HoriDir == EMoveDir::Right ? 1.f : -1.f));
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
	return EMoveDir::Forward;
}
