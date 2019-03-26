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

#define CLEAR_EFFECT(Effect)\
if (Effect)\
{\
	Effect->DestroyComponent();\
	Effect = nullptr;\
}

AActionGameCharacter_Countess::AActionGameCharacter_Countess():
	Enemy(nullptr),
	ShadowClone(nullptr),
	bFaceToEnemy(false)
{

}

void AActionGameCharacter_Countess::Ability_Q()
{
	if (ShadowClone)
	{
		SlipReturn();
		return;
	}

	if (IsAbilityinCooling(EAbilityType::QAbility) || bInAbility)return;
	else Super::Ability_Q();
		
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
	if (IsAbilityinCooling(EAbilityType::EAbility) || bInAbility)return;
	else Super::Ability_E();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[1], 1.f);
}

void AActionGameCharacter_Countess::Ability_R()
{
	if (IsAbilityinCooling(EAbilityType::RAbility) || bInAbility)return;
	else Super::Ability_R();
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	Super::Jump();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[2], 1.f);
	if (UltEyeFX)
	{
		UltEye_Right = UGameplayStatics::SpawnEmitterAttached(UltEyeFX, GetMesh(), TEXT("FX_R_eye"), FVector::ZeroVector);
		UltEye_Left = UGameplayStatics::SpawnEmitterAttached(UltEyeFX, GetMesh(), TEXT("FX_L_eye"), FVector::ZeroVector);
	}
	if (UltAimingFX)
	{
		UltAiming = UGameplayStatics::SpawnEmitterAttached(UltAimingFX, GetMesh(), TEXT("FX_SpineCenter"), FVector::ZeroVector);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionGameCharacter_Countess::ClearUltFX, 5.f, false);
}

void AActionGameCharacter_Countess::SpawnRollingDarkSegemnts()
{
	static const TArray<FVector> SegmentReltivePos = { FVector(300.f,0.f,0.f),FVector(150.f,100.f,0.f),FVector(150.f,-100.f,0.f),FVector(450.f,0.f,0.f),FVector(300.f,100.f,0.f),FVector(300.f,-100.f,0.f) };
	if (RollingDarkSegment)
	{
		const FVector LeftDir = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Y);
		for (auto RelPos : SegmentReltivePos)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RollingDarkSegment, GetActorLocation() + GetActorRotation().Vector()*RelPos.X - FVector(0.f, 0.f, 100.f) + LeftDir * RelPos.Y, GetActorRotation() + FRotator(0.f, -90.f, 0.f));
		}
	}
}

void AActionGameCharacter_Countess::StabEnemy()
{
	TArray<FHitResult> HitResult;
	const FVector StartPoint = GetActorLocation() + GetActorRotation().Vector()*50.f;
	const FVector EndPoint = StartPoint - FVector(0.f, 0.f, 200.f);
	//检测敌人
	GetWorld()->SweepMultiByObjectType(HitResult, StartPoint, EndPoint, FQuat::Identity, 
									   FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), 
		                               FCollisionShape::MakeSphere(40.f));

	for (auto Iter = HitResult.CreateIterator(); Iter; ++Iter)
	{
		if (Iter->GetActor() && Iter->GetActor() != this)
		{
			if (AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(Iter->GetActor()))
			{
				this->AttachToComponent(Enemy->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("Status"));
				this->GetCharacterMovement()->DisableMovement();
				HAIAIMIHelper::Debug_ScreenMessage(TEXT("Hit"));
			}
			break;
		}
	}
}

void AActionGameCharacter_Countess::MoveForward(float Value)
{
	Super::MoveForward(Value);
}

void AActionGameCharacter_Countess::MoveRight(float Value)
{
	Super::MoveRight(Value);
}

void AActionGameCharacter_Countess::Tick(float DeltaTime)
{
	if (bFaceToEnemy && Enemy)
	{
		//自动调整角度
		const FVector ToEnemyDir = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		float SubAngle = HAIAIMIHelper::AdaptSubAngle(GetActorRotation().Yaw, ToEnemyDir.Rotation().Yaw);
		const FRotator NewRotator = FMath::RInterpConstantTo(GetActorRotation(), ToEnemyDir.Rotation(), DeltaTime, 180.f);
		SetActorRotation(NewRotator);
	}
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

		CLEAR_EFFECT(ShadowClone)
		CLEAR_EFFECT(CountDown)
	});

	GetWorldTimerManager().SetTimer(TimerHandle, Delegate, 0.5f, false);
}

void AActionGameCharacter_Countess::ClearUltFX()
{
	CLEAR_EFFECT(UltAiming)
	CLEAR_EFFECT(UltEye_Left)
	CLEAR_EFFECT(UltEye_Right)

	if (UltEyeFX_Burst)
	{
		UGameplayStatics::SpawnEmitterAttached(UltEyeFX_Burst, GetMesh(), TEXT("FX_R_eye"), FVector::ZeroVector);
		UGameplayStatics::SpawnEmitterAttached(UltEyeFX_Burst, GetMesh(), TEXT("FX_L_eye"), FVector::ZeroVector);
	}
}
