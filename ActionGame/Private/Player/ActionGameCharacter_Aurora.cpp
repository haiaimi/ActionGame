// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameCharacter_Aurora.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>
#include <Engine/Engine.h>
#include <Animation/AnimInstance.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/SplineComponent.h>
#include "Components/CapsuleComponent.h"
#include <Components/BoxComponent.h>
#include "Common/HAIAIMIHelper.h"
#include "ActionAIController.h"
#include "Camera/CameraComponent.h"


AActionGameCharacter_Aurora::AActionGameCharacter_Aurora():
	bInAbilityJump(false),
	PreIcePlatform(nullptr),
	IcePlatformOffset(0.f),
	MoveTime(0.f),
	QFirstAttackTime(-5.f),
	AbilityJumpDir(0)
{
	NormalAttackAnims.SetNum(4);
	AbilityAnims.SetNum(3);
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.15f;

	SwordCollsion = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	IceMoveSpline = CreateDefaultSubobject<USplineComponent>(TEXT("IceMoveSpline"));
	QAbilityCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("QAbiilityCollision"));
	if (IceMoveSpline)
	{
		IceMoveSpline->SetupAttachment(RootComponent);
		IceMoveSpline->SetRelativeLocation(FVector(0.f, 0.f, -97.f));
		IceMoveSpline->bHiddenInGame = false;
	}

	if (SwordCollsion)
	{
		SwordCollsion->SetupAttachment(GetMesh(), TEXT("Sword_Mid"));
		SwordCollsion->SetBoxExtent(FVector(5.739f, 2.261f, 45.062f));
	}

	if(QAbilityCollision)
	{
		QAbilityCollision->SetupAttachment(GetMesh(), TEXT("Rush"));
		QAbilityCollision->SetBoxExtent(FVector(96.688881f, 56.212772f, 58.236698f));
		QAbilityCollision->SetRelativeLocation(FVector(70.f, 0.f, 60.f));
	}
}

void AActionGameCharacter_Aurora::FreezeEnemy()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParam;
	QueryParam.AddIgnoredActor(this);  //忽略自身
	GetWorld()->SweepMultiByObjectType(HitResults, GetActorLocation() + FVector(0.f, 0.f, 100.f),
									GetActorLocation() + FVector(0.f, 0.f, -500.f),
									FQuat::Identity,
									FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),   //只检测玩家
									FCollisionShape::MakeSphere(600.f),
									QueryParam);

	for (auto& Iter : HitResults)
	{
		if(Iter.GetActor())
		{
			FreezeEnemyImpl(Iter.GetActor());
		}
	}
}

void AActionGameCharacter_Aurora::FreezeEnemyImpl(class AActor* InEnemy)
{
	if (AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(InEnemy))
	{
		Enemy->bFreezedSlow = true;
		Enemy->ApplyFreezedParticle(Freezed_Slow);
		Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
		UGameplayStatics::SpawnEmitterAttached(CamFrostParticle_Slowed, Enemy->GetFollowCamera(), NAME_None, FVector(90.f, 0.f, 0.f),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset);
		GetWorldTimerManager().ClearTimer(FreezedSlowTimer);
		GetWorldTimerManager().SetTimer(FreezedSlowTimer, Enemy, &AActionGameCharacter::EndFreezedSlow, 2.f, false);
	}
}

void AActionGameCharacter_Aurora::BeginPlay()
{
	Super::BeginPlay();

	if (SwordCollsion)
		SwordCollsion->OnComponentBeginOverlap.AddDynamic(this, &AActionGameCharacter_Aurora::OnSwordBeginOverlap);
	if (QAbilityCollision)
		QAbilityCollision->OnComponentBeginOverlap.AddDynamic(this, &AActionGameCharacter_Aurora::OnQAbilityBeginOverlap);
}

void AActionGameCharacter_Aurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bTurboJumpAccelerate)
	{
		const FVector Direction = GetActorForwardVector();
		SetActorLocation(GetActorLocation() + Direction * 800.f * DeltaTime, false, nullptr, ETeleportType::ResetPhysics);
	}

	if (bInAbilityJump)
	{
		const FVector ForwardDirection = GetActorForwardVector();
		const FVector RightDirection = GetActorRightVector();
		TArray<FVector> Dirs = { ForwardDirection,-ForwardDirection,-RightDirection,RightDirection };
		FVector TmpMoveDir(0.f, 0.f, 0.f);
		for (int32 i = 3; i >= 0; --i)
		{
			if ((AbilityJumpDir >> i) & 1)TmpMoveDir += Dirs[i];
		}

		GetCharacterMovement()->MoveSmooth(TmpMoveDir.GetSafeNormal()*600.f, DeltaTime);
	}
}

void AActionGameCharacter_Aurora::Jump()
{
	if (bInAbility)return;
	Super::Jump();

	if (JumpCurrentCount == 1 && JumpCurrentCount < 2)
	{
		bTurboJumpAccelerate = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(TurboJumpAnim, 1.f);
		ResetCombo();
	}
}

void AActionGameCharacter_Aurora::MoveForward(float Value)
{
	if (bInAbility)return;
	if (JumpCurrentCount != 2)
		Super::MoveForward(Value);
}

void AActionGameCharacter_Aurora::MoveRight(float Value)
{
	if (bInAbility)return;
	if (JumpCurrentCount != 2)
		Super::MoveRight(Value);
}

void AActionGameCharacter_Aurora::Ability_Q()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || bFreezedStop || IsAbilityinCooling(EAbilityType::QAbility))return;
	Super::Ability_Q();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[0], 1.f);
	ResetCombo();   //重置攻击动画状态
	
	QAbilityCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bUseControllerRotationYaw = false;
	DetectIceRoad();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionGameCharacter_Aurora::SpawnIcePlatform, 0.3f, false);
}

void AActionGameCharacter_Aurora::Ability_E()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || IsAbilityinCooling(EAbilityType::EAbility))return;
	Super::Ability_E();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[1], 1.f);
	ResetCombo();   //重置攻击动画状态
	FVector Tmp = GetActorLocation();
	Tmp.Z = 50.f;
	FreezeTransform = FTransform(FRotator(0.f, GetActorRotation().Yaw - 180.f, GetActorRotation().Yaw - 180.f), Tmp);
	EmitFreeze();
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.Owner = this;
	if (EAbilityDetection)
		GetWorld()->SpawnActor<AActor>(EAbilityDetection, GetMesh()->GetSocketTransform(TEXT("Root")), SpawnParameter);
}

void AActionGameCharacter_Aurora::Ability_R()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || bTurboJumpAccelerate || IsAbilityinCooling(EAbilityType::RAbility) || bFreezedSlow || bFreezedStop)return;
	Super::Ability_R();

	GetCharacterMovement()->JumpZVelocity = 800.f;
	Super::Jump();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[2], 1.f);
	ResetCombo();   

	float FirstYaw = 0.f;
	const FVector CenterPos = GetActorLocation() - FVector(0.f, 0.f, 90.f);
	
	for (int32 i = 0; i < 6; ++i)
	{
		const FVector SpawnLocation = CenterPos + FRotator(0.f, FirstYaw, 0.f).Vector()*50.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RAbilityParticle, SpawnLocation, FRotator::ZeroRotator);
		FirstYaw += 60.f;
	}
}

void AActionGameCharacter_Aurora::Ability_F()
{
	if (IsAbilityinCooling(EAbilityType::FAbility) || FAbilityAnims.Num() < 8 || bInAbility)return;
	else Super::Ability_F();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AbilityJumpDir = GetMoveDirStat();
	
	int32 AnimIndex = 0;
	switch (AbilityJumpDir)
	{
		case 1: AnimIndex = 0;      //前
			break;
		case 2: AnimIndex = 1;      //后
			break;
		case 4: AnimIndex = 2;      //左
			break;
		case 8: AnimIndex = 3;      //右
			break;
		case 5: AnimIndex = 4;      //左前
			break;
		case 9: AnimIndex = 5;      //右前
			break;
		case 6: AnimIndex = 6;      //左后
			break;
		case 10: AnimIndex = 7;      //右后
			break;

		default:
		{
			AnimIndex = 0;
			AbilityJumpDir = 1;
		}
	}
	
	if (AnimInstance)AnimInstance->Montage_Play(FAbilityAnims[AnimIndex]);
	bInAbilityJump = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	Super::Jump();
}

bool AActionGameCharacter_Aurora::HitReact(const FVector& HitPoint)
{
	if(!bInAbilityJump && Super::HitReact(HitPoint))
	{
		ResetCombo();
		if (bInAbility)
		{
			bInAbility = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetCharacterMovement()->JumpZVelocity = 600.f;
			bTurboJumpAccelerate = false;
			QAbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		return true;
	}
	return false;
}

void AActionGameCharacter_Aurora::EmitFreeze()
{
	FTimerHandle EmitHandle;
	FRotator CurRot = FreezeTransform.Rotator();
	if (CurRot.Pitch > 12.f)
		return;

	CurRot.Pitch = 0.f;
	CurRot.Roll = 0.f;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FreezeSegment, FreezeTransform.GetLocation() + CurRot.Vector()*300.f, FRotationMatrix(CurRot).GetUnitAxis(EAxis::Y).ToOrientationRotator());
	GetWorldTimerManager().SetTimer(EmitHandle, this, &AActionGameCharacter_Aurora::EmitFreeze, 0.05f, false);

	CurRot = FreezeTransform.Rotator();
	CurRot.Yaw += 30.f;
	CurRot.Pitch += 1.f;
	FreezeTransform.SetRotation(FQuat(CurRot));
}

//生成冰块路径
void AActionGameCharacter_Aurora::SpawnIcePlatform()
{
	if (MoveTime == 0.f && bInAbility && !bFreezedStop)
		bTurboJumpAccelerate = true;

	if (800.f*MoveTime > IceMoveSpline->GetSplineLength() || !bInAbility)
	{
		bTurboJumpAccelerate = false;
		MoveTime = 0.f;
		return;
	}
	FTimerHandle TimerHandle;
	FVector SpawnLocation;
	FRotator SpawnRotation = GetActorRotation();

	SpawnLocation = IceMoveSpline->GetLocationAtDistanceAlongSpline(800.f*MoveTime, ESplineCoordinateSpace::World);
	SpawnRotation = IceMoveSpline->GetRotationAtDistanceAlongSpline(800.f*MoveTime, ESplineCoordinateSpace::World);
	SpawnLocation.Z -= IcePlatformOffset;
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(IcePlatform, SpawnLocation, SpawnRotation, SpawnParameter);
	MoveTime += 0.05f;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionGameCharacter_Aurora::SpawnIcePlatform, 0.05f, false);
}

//检测路况
void AActionGameCharacter_Aurora::DetectIceRoad()
{
	const float OffsetLength = 180.f;
	IceMoveSpline->ClearSplinePoints();    //清除所有已有的关键点
	IceMoveSpline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IceMoveSpline->SetRelativeLocation(FVector(0.f, 0.f, -97.f));
	IceMoveSpline->SetRelativeRotation(FRotator::ZeroRotator);
	const FVector RoadDir = GetActorRotation().Vector();
	const FVector StartPoint = GetActorLocation() + RoadDir * OffsetLength;
	
	for (int32 i = 0; i < 20; ++i)
	{
		const FVector CurDetectPoint = StartPoint + RoadDir * i * 50.f;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult,
											 CurDetectPoint + FVector(0.f, 0.f, 500.f),     //检测起始位置，尽量靠上
											 CurDetectPoint + FVector(0.f, 0.f, -500.f),
											 COLLISION_ICETRACE,    //该通道已经在编辑器里设置为Ice专用检测通道
											 QueryParams);

		if(HitResult.bBlockingHit)
		{
			const FVector ImpactPoint = HitResult.ImpactPoint + HitResult.ImpactNormal*20.f;
			const FVector SplinePointPos = FVector(OffsetLength + i * 50.f, 0.f, ImpactPoint.Z);
			IceMoveSpline->AddPoint(FSplinePoint(i, SplinePointPos, ESplinePointType::Curve));
		}
	}

	IceMoveSpline->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	IcePlatformOffset = IceMoveSpline->GetComponentLocation().Z;
	IceMoveSpline->SetWorldRotation(FRotator(0.f, IceMoveSpline->K2_GetComponentRotation().Yaw, IceMoveSpline->K2_GetComponentRotation().Roll));
}

void AActionGameCharacter_Aurora::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bCanAttack)return;
	AttackEnemy(OtherComp, OtherActor, TEXT("Sword_Mid"));
}

void AActionGameCharacter_Aurora::OnQAbilityBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Cast<AActionGameCharacter>(OtherActor) && AnimInstance->Montage_IsPlaying(AbilityAnims[0]) && bInAbility && GetWorld()->TimeSeconds - QFirstAttackTime >= 1.5f)
	{
		//HAIAIMIHelper::Debug_ScreenMessage(FString::SanitizeFloat(GetWorld()->TimeSeconds));
		AttackEnemy(OverlappedComponent, OtherActor, TEXT("Sword_Mid"));
		QFirstAttackTime = GetWorld()->TimeSeconds;
	}
}

void AActionGameCharacter_Aurora::DisableQDetection()
{
	QAbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AActionGameCharacter_Aurora::EffectOfAbilityE(class AActor* InEnemy)
{
	if (InEnemy == this)return;   //忽略玩家自身
	if(AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(InEnemy))
	{
		const bool bPreFreezedSlowStat = Enemy->bFreezedSlow;
		FreezeEnemyImpl(InEnemy);
		Enemy->bFreezedSlow = bPreFreezedSlowStat;
	}
}

AActionGameCharacter* AActionGameCharacter_Aurora::AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket)
{
	if (AActionGameCharacter* Enemy = Super::AttackEnemy(OverlappedComponent, OtherActor, SwordSocket))
	{
		if (Enemy->IsDead())
		{
			if(Enemy->bFreezedStop && ExplodeParticle_Frozen)
				UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticle_Frozen, Enemy->GetActorLocation(),FRotator::ZeroRotator);
			if (Enemy->bFreezedSlow && ExplodeParticle_Slow)
				UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticle_Slow, Enemy->GetActorLocation(), FRotator::ZeroRotator);
			Enemy->SetActorHiddenInGame(Enemy->bFreezedStop || Enemy->bFreezedSlow);
			return Enemy;
		}

		if (Enemy->bFreezedSlow)
		{
			Enemy->ApplyFreezedParticle(Freezed_Stop);
			Enemy->bFreezedSlow = false;
			//停止玩家移动及相关动画
			Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			Enemy->GetMesh()->bNoSkeletonUpdate = true;
			Enemy->bFreezedStop = true;
			Enemy->EnableAttack(false);

			Enemy->bUseControllerRotationRoll = false;
			if (AActionAIController* AIControl = Cast<AActionAIController>(Enemy->Controller))
			{
				AIControl->SetAIFreezedValue();
			}
			else
			{
				UGameplayStatics::SpawnEmitterAttached(CamFrostParticle_Frozen, Enemy->GetFollowCamera(), NAME_None, FVector(90.f, 0.f, 0.f),FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset);
			}
					
			FTimerHandle TimerHandle;
			/*FTimerDelegate TimerDelegate;
			TimerDelegate.BindWeakLambda(this, [Enemy, this]() {
				Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				Enemy->GetMesh()->bNoSkeletonUpdate = false;
				Enemy->GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
				Enemy->bFreezedStop = false;
				Enemy->ResetCombo();
				Enemy->bInAbility = false;
				if (AActionAIController* AIControl = Cast<AActionAIController>(Enemy->Controller))
					AIControl->SetAIFreezedValue();
				});*/

			GetWorldTimerManager().SetTimer(TimerHandle, Enemy, &AActionGameCharacter::EndFreezedStop, 2.f, false);
		}
	}

	return nullptr;
}