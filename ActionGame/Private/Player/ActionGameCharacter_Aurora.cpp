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


AActionGameCharacter_Aurora::AActionGameCharacter_Aurora():
	SaveAttack(false),
	IsAttacking(false),
	AttackCount(0),
	bTurboJumpAccelerate(false),
	PreIcePlatform(nullptr),
	IcePlatformOffset(0.f),
	bCanAttack(false),
	MoveTime(0.f)
{
	NormalAttackAnims.SetNum(4);
	AbilityAnims.SetNum(3);
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.15f;

	SwordCollsion = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	IceMoveSpline = CreateDefaultSubobject<USplineComponent>(TEXT("IceMoveSpline"));
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
}

void AActionGameCharacter_Aurora::ComboAttackSave()
{
	if(SaveAttack)
	{
		SaveAttack = false;
		if (AttackCount < NormalAttackAnims.Num()-1)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(NormalAttackAnims[AttackCount++], 1.f);
		}
	}
}

void AActionGameCharacter_Aurora::ResetCombo()
{
	SaveAttack = false;
	IsAttacking = false;
	AttackCount = 0;
	bCanAttack = false;

	Super::ResetCombo();
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
			if (AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(Iter.GetActor()))
			{
				Enemy->bFreezedSlow = true;
				Enemy->ApplyFreezedParticle(Freezed_Slow);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;

				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([Enemy, this]() {
					Enemy->bFreezedSlow = false;
					Enemy->GetCharacterMovement()->MaxWalkSpeed = 600.f;
					});

				GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, false);
			}
		}
	}
}

void AActionGameCharacter_Aurora::BeginPlay()
{
	Super::BeginPlay();

	if (SwordCollsion)
		SwordCollsion->OnComponentBeginOverlap.AddDynamic(this, &AActionGameCharacter_Aurora::OnSwordBeginOverlap);
}

void AActionGameCharacter_Aurora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bTurboJumpAccelerate)
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		SetActorLocation(GetActorLocation() + Direction * 800.f * DeltaTime, false, nullptr, ETeleportType::ResetPhysics);
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

void AActionGameCharacter_Aurora::NormalAttack()
{
	if (bInAbility || bTurboJumpAccelerate)return;   //在释放技能的时候不进行平A

	if (IsAttacking)
		SaveAttack = true;
	else
	{
		IsAttacking = true;
		if (AttackCount < NormalAttackAnims.Num()-1)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (GetCharacterMovement()->IsFalling())
				AnimInstance->Montage_Play(NormalAttackAnims.Last(), 1.f);
			else
				AnimInstance->Montage_Play(NormalAttackAnims[AttackCount++], 1.f);
		}
	}
}

void AActionGameCharacter_Aurora::Ability_Q()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || bFreezedStop || IsAbilityinCooling(0))return;
	Super::Ability_Q();

	bInAbility = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[0], 1.f);
	ResetCombo();   //重置攻击动画状态
	
	bUseControllerRotationYaw = false;
	DetectIceRoad();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionGameCharacter_Aurora::SpawnIcePlatform, 0.3f, false);
}

void AActionGameCharacter_Aurora::Ability_E()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || IsAbilityinCooling(1))return;
	Super::Ability_E();

	bInAbility = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AbilityAnims[1], 1.f);
	ResetCombo();   //重置攻击动画状态
	FVector Tmp = GetActorLocation();
	Tmp.Z = 50.f;
	FreezeTransform = FTransform(FRotator(0.f, GetActorRotation().Yaw - 180.f, GetActorRotation().Yaw - 180.f), Tmp);
	EmitFreeze();
}

void AActionGameCharacter_Aurora::Ability_R()
{
	if (GetCharacterMovement()->IsFalling() || bInAbility || bTurboJumpAccelerate || IsAbilityinCooling(2))return;
	Super::Ability_R();

	GetCharacterMovement()->JumpZVelocity = 700.f;
	Jump();
	bInAbility = true;
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

bool AActionGameCharacter_Aurora::HitReact(const FVector& HitPoint)
{
	if(Super::HitReact(HitPoint))
	{
		ResetCombo();
		if (bInAbility)
		{
			bInAbility = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			bTurboJumpAccelerate = false;
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
	//首先对前方路况检测
	FHitResult HitResult1, HitResult2;
	
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
											 ECollisionChannel::ECC_GameTraceChannel1,    //该通道已经在编辑器里设置为Ice专用检测通道
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
	HAIAIMIHelper::Debug_ScreenMessage(IceMoveSpline->K2_GetComponentRotation().ToString());
	IceMoveSpline->SetWorldRotation(FRotator(0.f, IceMoveSpline->K2_GetComponentRotation().Yaw, IceMoveSpline->K2_GetComponentRotation().Roll));
}

void AActionGameCharacter_Aurora::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bCanAttack)return;

	if (AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(OtherActor))
	{
		if (Enemy != this)
		{
			UGameplayStatics::SpawnEmitterAttached(ImpactParticle, Enemy->GetMesh(), TEXT("Impact"));

			//获取最适合的打击点
			FVector NewImpactPoint, NewImpactNormal;
			if (UBodySetup* BodySetup = OverlappedComponent->GetBodySetup())
			{
				BodySetup->GetClosestPointAndNormal(GetMesh()->GetSocketLocation(TEXT("Sword_Mid")), OverlappedComponent->GetComponentTransform(), NewImpactPoint, NewImpactNormal);
			}

			Enemy->HitReact(NewImpactPoint);
			bCanAttack = false;
			if (Enemy->bFreezedSlow)
			{
				Enemy->ApplyFreezedParticle(Freezed_Stop);
				Enemy->bFreezedSlow = false;
				//停止玩家移动及相关动画
				Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
				Enemy->GetMesh()->bNoSkeletonUpdate = true;
				Enemy->bFreezedStop = true;
				Enemy->bUseControllerRotationRoll = false;
				if (AActionAIController* AIControl = Cast<AActionAIController>(Enemy->Controller))
				{
					AIControl->SetAIFreezedValue();
				}
					
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([Enemy, this]() {
					Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					Enemy->GetMesh()->bNoSkeletonUpdate = false;
					Enemy->GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
					Enemy->bFreezedStop = false;
					Enemy->ResetCombo();
					if (AActionAIController* AIControl = Cast<AActionAIController>(Enemy->Controller))
					{
						AIControl->SetAIFreezedValue();
					}
					});

				GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, false);
			}
		}
	}
}
