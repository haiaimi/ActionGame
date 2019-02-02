// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ActionGameBot_Aurora.h"
#include "AI/ActionAIController.h"
#include "HAIAIMIHelper.h"

AActionGameBot_Aurora::AActionGameBot_Aurora():
	AIMoveDir(EMoveDir::Forward)
{
	AIControllerClass = AActionAIController::StaticClass();

	bUseControllerRotationYaw = true;
}

void AActionGameBot_Aurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//¸üÐÂAI Rotator
	const FRotator CurRot = GetActorRotation();
	if (IsMoveBack())
	{
		FRotator NewRot = FMath::RInterpConstantTo(CurRot, TempRot, DeltaTime, 180.0f);
		SetActorRotation(NewRot);
	}
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

void AActionGameBot_Aurora::SetAIRotation(FRotator Rotator)
{
	bUpdateRotation = true;
	TempRot = Rotator;
}

EMoveDir::Type AActionGameBot_Aurora::GetMoveDirection()
{
	return AIMoveDir;
}

void AActionGameBot_Aurora::SetMoveDir(EMoveDir::Type InDir)
{
	AIMoveDir = InDir;
}

bool AActionGameBot_Aurora::IsMoveBack()
{
	AActionAIController* MyController = Cast<AActionAIController>(Controller);
	if (MyController)return MyController->IsMovingBack();
	return false;
}
