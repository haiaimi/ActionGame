// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameCharacter_Countess.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "HAIAIMIHelper.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"

AActionGameCharacter_Countess::AActionGameCharacter_Countess():
	ShadowClone(nullptr)
{

}

void AActionGameCharacter_Countess::Ability_Q()
{
	if (IsAttacking)return;

	if (ShadowClone)
	{
		SlipReturn();
		return;
	}
		
	if (BeginTeleportEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeginTeleportEffect, GetActorLocation() - FVector(0.f, 0.f, 50.f), GetActorRotation());
	if (TeleportShadowClone)
		ShadowClone = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportShadowClone, GetActorLocation(), GetActorRotation());
	if (SlipTimerCountDown)
	{
		CountDown = UGameplayStatics::SpawnEmitterAttached(SlipTimerCountDown, GetFollowCamera(), NAME_None, FVector(120.f, 0.f, 0.f),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset);
		CountDown->OnParticleDeath.AddDynamic(this, &AActionGameCharacter_Countess::OnCountDownFinshed);
	}

	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	FTransform CurTransform = GetActorTransform();
	SetActorHiddenInGame(true);
	Delegate.BindLambda([CurTransform,this]() {
		SetActorHiddenInGame(false);
		TeleportTo(CurTransform.GetLocation() + 500.f* CurTransform.GetRotation().Vector(), CurTransform.GetRotation().Rotator());
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(AbilityAnims[0], 1.f);
		if (TeleportCamEffect)
			UGameplayStatics::SpawnEmitterAttached(TeleportCamEffect, GetFollowCamera(), NAME_None, FVector(150.f, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	});
	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, false);
}

void AActionGameCharacter_Countess::Ability_E()
{
	if (IsAttacking)return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[1], 1.f);
}

void AActionGameCharacter_Countess::Ability_R()
{
	if (IsAttacking)return;
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	Super::Jump();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[2], 1.f);
}

void AActionGameCharacter_Countess::OnCountDownFinshed(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction)
{
	if (ShadowClone)
	{
		if (TeleportShadowBurst)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportShadowBurst, ShadowClone->GetComponentTransform());
		ShadowClone->DestroyComponent();
		ShadowClone = nullptr;
		CountDown = nullptr;
	}
}

void AActionGameCharacter_Countess::SlipReturn()
{
	if (TeleportReturnEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportReturnEffect, GetActorLocation(), GetActorRotation());
	if (TeleportReturnCamEffect)
		UGameplayStatics::SpawnEmitterAttached(TeleportReturnCamEffect, GetFollowCamera(), NAME_None, FVector(90.f, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	
	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	FTransform PreTransform = ShadowClone->GetComponentTransform();
	SetActorHiddenInGame(true);
	DisableInput(GetController<APlayerController>());
	Delegate.BindLambda([PreTransform, this]() {
		EnableInput(GetController<APlayerController>());
		SetActorHiddenInGame(false);
		TeleportTo(PreTransform.GetLocation() + PreTransform.GetRotation().Vector(), PreTransform.GetRotation().Rotator());

		if (ShadowClone)
		{
			ShadowClone->DestroyComponent();
			ShadowClone = nullptr;
		}
		if (CountDown)
		{
			CountDown->DestroyComponent();
			CountDown = nullptr;
		}
	});

	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, false);
}
