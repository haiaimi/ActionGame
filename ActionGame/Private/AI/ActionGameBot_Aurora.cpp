// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ActionGameBot_Aurora.h"
#include "AI/ActionAIController.h"

AActionGameBot_Aurora::AActionGameBot_Aurora():
	AIMoveDir(EMoveDir::Forward)
{
	AIControllerClass = AActionAIController::StaticClass();

	bUseControllerRotationYaw = true;
}

void AActionGameBot_Aurora::FaceRotation(FRotator NewRotation, float DeltaTime /*= 0.f*/)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 4.0f);

	ACharacter::FaceRotation(CurrentRotation, DeltaTime);
}

void AActionGameBot_Aurora::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce /*=false*/)
{
	ACharacter::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

EMoveDir::Type AActionGameBot_Aurora::GetMoveDirection()
{
	return AIMoveDir;
}

void AActionGameBot_Aurora::SetMoveDir(EMoveDir::Type InDir)
{
	AIMoveDir = InDir;
}
