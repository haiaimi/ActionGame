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
#include "EngineUtils.h"
#include "Components/BoxComponent.h"

#define CLEAR_EFFECT(Effect)\
if (Effect)\
{\
	Effect->DestroyComponent();\
	Effect = nullptr;\
}

AActionGameCharacter_Countess::AActionGameCharacter_Countess():
	ShadowClone(nullptr),
	bFaceToEnemy(false),
	SwordCollision_L(nullptr),
	SwordCollision_R(nullptr)
{
#if WITH_EDITOR
	GetEnemy(true);
#endif

	SwordCollision_L = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision_L"));
	SwordCollision_R = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision_R"));
	if (SwordCollision_L)
	{
		SwordCollision_L->SetupAttachment(GetMesh(), TEXT("weapon_mid_l"));
		SwordCollision_L->SetBoxExtent(FVector(3.1762f, 32.4572f, 3.1681f));
	}
		
	if (SwordCollision_R)
	{
		SwordCollision_R->SetupAttachment(GetMesh(), TEXT("weapon_mid_r"));
		SwordCollision_R->SetBoxExtent(FVector(3.1762f, 32.4572f, 3.1681f));
	}
}

void AActionGameCharacter_Countess::BeginPlay()
{
	Super::BeginPlay();
	if (SwordCollision_L)SwordCollision_L->OnComponentBeginOverlap.AddDynamic(this, &AActionGameCharacter_Countess::OnSwordBeginOverlap);
	if (SwordCollision_R)SwordCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AActionGameCharacter_Countess::OnSwordBeginOverlap);
}

void AActionGameCharacter_Countess::Tick(float DeltaTime)
{
	if (bFaceToEnemy && GetEnemy())
	{
		//自动调整角度
		const FVector ToEnemyDir = (GetEnemy()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		const FRotator NewRotator = FMath::RInterpConstantTo(GetActorRotation(), ToEnemyDir.Rotation(), DeltaTime, 180.f);
		SetActorRotation(NewRotator);
	}
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
	DisableInput(GetController<APlayerController>());
	SetActorHiddenInGame(true);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionGameCharacter_Countess::TeleportArrive, 0.5f, false);
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
	static const TArray<FVector> SegmentReltivePos = { FVector(300.f,0.f,0.f),FVector(150.f,100.f,0.f),FVector(150.f,-100.f,0.f),
												       FVector(450.f,0.f,0.f),FVector(300.f,100.f,0.f),FVector(300.f,-100.f,0.f),
													   FVector(600.f,0.f,0.f),FVector(450.f,100.f,0.f),FVector(450.f,-100.f,0.f) };
	if (RollingDarkSegment)
	{
		const FVector LeftDir = FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Y);
		for (auto RelPos : SegmentReltivePos)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RollingDarkSegment, GetActorLocation() + GetActorRotation().Vector()*RelPos.X - FVector(0.f, 0.f, 100.f) + LeftDir * RelPos.Y, GetActorRotation() + FRotator(0.f, -90.f, 0.f));
		}
	}

	//对敌人施加伤害
	TArray<FHitResult> Results;
	const FVector Dir = GetActorRotation().Vector();
	const FVector StartPos = GetActorLocation() + 100.f * Dir - FVector(0.f, 0.f, 90.f);
	GetWorld()->SweepMultiByObjectType(Results, StartPos,
		StartPos + Dir * 500.f,
		FQuat(GetActorRotation()),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		FCollisionShape::MakeBox(FVector(10.f, 50.f, 50.f)));

	for (auto Iter : Results)
	{
		if (Iter.GetActor() && Iter.GetActor() != this)
		{
			auto Enemy = Cast<AActionGameCharacter>(Iter.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RollingDarkImpact, Enemy->GetActorLocation() - FVector(0.f, 0.f, 100.f), Enemy->GetActorRotation());
			AttackEnemy(Enemy->GetMesh(), Enemy);
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
				AttachToComponent(Enemy->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("Status"));
				GetCharacterMovement()->DisableMovement();
				AttackEnemy(Enemy->GetMesh(), Enemy);
			}
			break;
		}
	}
}

class AActionGameCharacter* AActionGameCharacter_Countess::GetEnemy(bool ClearPre)
{
	static AActionGameCharacter* Enemy = nullptr;
	if (ClearPre)
	{
		Enemy = nullptr;
		return nullptr;
	}
	if (Enemy && Enemy != this)return Enemy;

	for (TActorIterator<AActionGameCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		if (*Iter == this)continue;
		Enemy = *Iter;
	}
	return Enemy;
}

void AActionGameCharacter_Countess::MoveForward(float Value)
{
	Super::MoveForward(Value);
}

void AActionGameCharacter_Countess::MoveRight(float Value)
{
	Super::MoveRight(Value);
}

AActionGameCharacter* AActionGameCharacter_Countess::AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket /*= NAME_None*/)
{
	auto Enemy = Super::AttackEnemy(OverlappedComponent, OtherActor, SwordSocket);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (Enemy && AnimInstance && AnimInstance->Montage_IsPlaying(AbilityAnims[2]))
		UGameplayStatics::SpawnEmitterAttached(UltImpactFX, Enemy->GetMesh(), TEXT("Impact"));
	return Enemy;
}

void AActionGameCharacter_Countess::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bCanAttack)return;
	AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(OtherActor);
	if (!Enemy || Enemy == this)return;

	HAIAIMIHelper::Debug_ScreenMessage(FString::FormatAsNumber(AttackCount));
	if (AttackCount == NormalAttackAnims.Num())AttackCount = 3;
	switch (AttackCount)
	{
		case 1:
		{
			if (OverlappedComponent == SwordCollision_R)
				AttackEnemy(OtherComp, OtherActor, TEXT("weapon_mid_r"));
			break;
		}
		case 2:
		{
			if (OverlappedComponent == SwordCollision_L)
				AttackEnemy(OtherComp, OtherActor, TEXT("weapon_mid_l"));
			break;
		}
		case 3:
		{
			bool IsRight = OverlappedComponent == SwordCollision_R;
			AttackEnemy(OtherComp, OtherActor, IsRight ? TEXT("weapon_mid_r") : TEXT("weapon_mid_l"));
			break;
		}
	default:
		break;
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

void AActionGameCharacter_Countess::TeleportArrive()
{
	auto Enemy = GetEnemy();
	const float SubDegree = HAIAIMIHelper::GetDegreesBetweenActors(Enemy, this);
	FVector AimPos = GetActorLocation() + 1000.f * GetActorRotation().Vector();
	if (SubDegree < 45.f)
	{
		const FVector Dir = (GetEnemy()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const float Distance = (GetEnemy()->GetActorLocation() - GetActorLocation()).Size();
		AimPos = GetActorLocation() + Dir * FMath::Clamp(Distance - 100.f, 0.f, 1000.f);
	}

	EnableInput(GetController<APlayerController>());
	SetActorHiddenInGame(false);
	TeleportTo(AimPos, GetActorRotation());
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[0], 1.f);
	if (TeleportCamEffect)
		UGameplayStatics::SpawnEmitterAttached(TeleportCamEffect, GetFollowCamera(), NAME_None, FVector(150.f, 0.f, 0.f), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
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