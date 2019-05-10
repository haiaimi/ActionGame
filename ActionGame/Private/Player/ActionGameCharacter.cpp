// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Engine/Engine.h>
#include <TimerManager.h>
#include "Common/HAIAIMIHelper.h"
#include <Particles/ParticleSystem.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SkeletalMeshComponent.h>
#include "Engine/SkeletalMesh.h"
#include <Animation/AnimInstance.h>
#include "AI/ActionAIController.h"
#include "ActionGameInstance.h"
#include "AGPlayerController.h"
#include "Sound/SoundCue.h"
//#include "ActionGame.h"

//////////////////////////////////////////////////////////////////////////
// AActionGameCharacter

AActionGameCharacter::AActionGameCharacter():
	bCheatMode(false),
	bInAbility(false),
	bTurboJumpAccelerate(false),
	bFreezedSlow(false),
	bFreezedStop(false),
	SaveAttack(false),
	IsAttacking(false),
	bCanAttack(false),
	AttackCount(0),
	MoveDirStat(0),
	bDead(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->ProbeChannel = COLLISION_ICETRACE;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SkillCoolingTimes.SetNum(4);
	SkillCoolingTimers.SetNum(SkillCoolingTimes.Num());
}

void AActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!bCheatMode)
		DisableInput(GetController<APlayerController>());
	if(UActionGameInstance* MyGameInstance = Cast<UActionGameInstance>(GetGameInstance()))
	{
		if (AIControllerClass == AActionAIController::StaticClass())
		{
			int32 AISkinIndex = FMath::RandRange(0, CharacterMeshes.Num() - 1);
			if (MyGameInstance->EnemyIndex == MyGameInstance->PlayerIndex && AISkinIndex == MyGameInstance->PlayerSkinIndex)
				AISkinIndex = (AISkinIndex + 1) % CharacterMeshes.Num();
			GetMesh()->SetSkeletalMesh(CharacterMeshes[AISkinIndex]);
			return;
		}
		if (MyGameInstance->PlayerSkinIndex < CharacterMeshes.Num())
		{
			GetMesh()->SetSkeletalMesh(CharacterMeshes[MyGameInstance->PlayerSkinIndex]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Ability_Q", IE_Pressed, this, &AActionGameCharacter::Ability_Q);
	PlayerInputComponent->BindAction("Ability_E", IE_Pressed, this, &AActionGameCharacter::Ability_E);
	PlayerInputComponent->BindAction("Ability_R", IE_Pressed, this, &AActionGameCharacter::Ability_R);
	PlayerInputComponent->BindAction("Ability_F", IE_Pressed, this, &AActionGameCharacter::Ability_F);
	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &AActionGameCharacter::NormalAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AActionGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActionGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AActionGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AActionGameCharacter::LookUpAtRate);
}

void AActionGameCharacter::TurnAtRate(float Rate)
{
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AActionGameCharacter::LookUpAtRate(float Rate)
{
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

AActionGameCharacter* AActionGameCharacter::AttackEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, FName SwordSocket /*= NAME_None*/)
{
	if (AActionGameCharacter* Enemy = Cast<AActionGameCharacter>(OtherActor))
	{
		if (Enemy != this && !Enemy->IsDead() && !Enemy->bHidden)
		{
			UGameplayStatics::SpawnEmitterAttached(ImpactParticle, Enemy->GetMesh(), TEXT("Impact"));
			const float EnemyHealth = Enemy->TakeDamage(60.f, FDamageEvent(), GetController(), this);
			//获取最适合的打击点```
			FVector NewImpactPoint, NewImpactNormal;
			if (UBodySetup* BodySetup = OverlappedComponent->GetBodySetup())
			{
				BodySetup->GetClosestPointAndNormal(GetMesh()->GetSocketLocation(SwordSocket), OverlappedComponent->GetComponentTransform(), NewImpactPoint, NewImpactNormal);
			}
			Enemy->HitReact(NewImpactPoint);
			bCanAttack = false;
			return Enemy;    //本次攻击导致死亡也会返回
		}
	}
	return nullptr;
}

void AActionGameCharacter::MakeAbilityCooling(EAbilityType::Type AbilityType)
{
	if (IsAttacking)
		ResetCombo();    //进入释放技能阶段先重置普攻
	const int32 Index = int32(AbilityType) - 1;
	if (Index >= SkillCoolingTimes.Num())return;
	bInAbility = true;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([Index, this]() {
		SetAbilityReady(Index);
		});
	GetWorldTimerManager().SetTimer(SkillCoolingTimers[Index], TimerDelegate, SkillCoolingTimes[Index], false);
	SkillCoolingTimes[Index] = 0.f;
}

void AActionGameCharacter::SetAbilityReady(int32 Index)
{
	const float DefaultCoolingTime = GetClass()->GetDefaultObject<AActionGameCharacter>()->SkillCoolingTimes[Index];
	SkillCoolingTimes[Index] = DefaultCoolingTime;
}

void AActionGameCharacter::NormalAttack()
{
	if (bInAbility || bTurboJumpAccelerate)return;   //在释放技能的时候不进行平A

	if (IsAttacking)
		SaveAttack = true;
	else
	{
		IsAttacking = true;
		if (AttackCount < NormalAttackAnims.Num() - 1)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (GetCharacterMovement()->IsFalling())
			{
				AnimInstance->Montage_Play(NormalAttackAnims.Last(), 1.f);     //播放空中普攻的动画
				AttackCount = NormalAttackAnims.Num();
			}
			else
				AnimInstance->Montage_Play(NormalAttackAnims[AttackCount++], 1.f);

			if (AttackSound)
				UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
			bCanAttack = true;
		}
	}
}

void AActionGameCharacter::ComboAttackSave()
{
	if(SaveAttack)
	{
		SaveAttack = false;
		if (AttackCount < NormalAttackAnims.Num() - 1)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(NormalAttackAnims[AttackCount++], 1.f);
			if (AttackSound)
				UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
			bCanAttack = true;
		}
	}
}

void AActionGameCharacter::ResetCombo()
{
	SaveAttack = false;
	IsAttacking = false;
	AttackCount = 0;
	bCanAttack = false;
}

void AActionGameCharacter::Ability_Q()
{
	MakeAbilityCooling(EAbilityType::QAbility);
	if (AbilitySounds.Num() >= EAbilityType::QAbility)
		UGameplayStatics::PlaySoundAtLocation(this, AbilitySounds[int32(EAbilityType::QAbility) - 1], GetActorLocation());
}

void AActionGameCharacter::Ability_E()
{
	MakeAbilityCooling(EAbilityType::EAbility);
	if (AbilitySounds.Num() >= EAbilityType::EAbility)
		UGameplayStatics::PlaySoundAtLocation(this, AbilitySounds[int32(EAbilityType::EAbility) - 1], GetActorLocation());
}

void AActionGameCharacter::Ability_R()
{
	MakeAbilityCooling(EAbilityType::RAbility);
	if (AbilitySounds.Num() >= EAbilityType::RAbility)
		UGameplayStatics::PlaySoundAtLocation(this, AbilitySounds[int32(EAbilityType::RAbility) - 1], GetActorLocation());
}

void AActionGameCharacter::Ability_F()
{
	MakeAbilityCooling(EAbilityType::FAbility);
	if (AbilitySounds.Num() >= EAbilityType::FAbility)
		UGameplayStatics::PlaySoundAtLocation(this, AbilitySounds[int32(EAbilityType::FAbility) - 1], GetActorLocation());
}

bool AActionGameCharacter::IsAbilityinCooling(EAbilityType::Type AbilityType)
{
	const int32 Index = int32(AbilityType) - 1;
	return SkillCoolingTimes[Index] == 0.f;
}

bool AActionGameCharacter::IsInAbility(EAbilityType::Type AbilityType)
{ 
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
		return AnimInstance->Montage_IsPlaying(AbilityAnims[int32(AbilityType) - 1]);
	return false;
}

float AActionGameCharacter::GetCoolingRate(EAbilityType::Type AbilityType)
{
	const int32 Index = int32(AbilityType) - 1;
	
	if (Index >= SkillCoolingTimes.Num())return 1.f;
	const float DefaultCoolingTime = GetClass()->GetDefaultObject<AActionGameCharacter>()->SkillCoolingTimes[Index];
	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(SkillCoolingTimers[Index]);
	if (RemainingTime == -1)return 1.f;
	return 1.f - RemainingTime / DefaultCoolingTime;
}

AActionGameCharacter* AActionGameCharacter::GetAIEnemy()
{
	if (!GetWorld())return nullptr;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		if (It->Get() != this && It->Get()->AIControllerClass == AActionAIController::StaticClass())
		{
			return Cast<AActionGameCharacter>(It->Get());
		}	
	}
	return nullptr;
}

void AActionGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int32 i = 0; i < SkillCoolingTimers.Num(); ++i)
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillCoolingTimers[i]);
	}

	Super::EndPlay(EndPlayReason);
}

float AActionGameCharacter::GetRemainHealthRate()
{
	const float DefaultHealth = GetClass()->GetDefaultObject<AActionGameCharacter>()->Health;
	return Health / DefaultHealth;
}

void AActionGameCharacter::FaceRotation(FRotator NewRotation, float DeltaTime /*= 0.f*/)
{
	FRotator CurrentRotation;
	if (GetMoveDirection() == EMoveDir::Left || GetMoveDirection() == EMoveDir::Right || GetMoveDirection() == EMoveDir::Backward)
		CurrentRotation = NewRotation;
	else
		CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 4.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void AActionGameCharacter::MoveForward(float Value)
{
	if (bFreezedStop)
	{
		bUseControllerRotationYaw = false;
		return;
	}
	if (Value > 0.f)
	{
		MoveDirStat |= 1;
		MoveDirStat &= 13;
	}
	else if (Value < 0.f)
	{
		MoveDirStat |= 2;
		MoveDirStat &= 14;
	}
	else MoveDirStat &= 12;

	bool Tmp = (MoveDirStat == 0) || ((MoveDirStat & 1) && (MoveDirStat & 12));
	bUseControllerRotationYaw = !Tmp;

	if ((Controller != NULL) && (Value != 0.0f) && GetCharacterMovement()->GetMaxSpeed() > 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AActionGameCharacter::MoveRight(float Value)
{
	if (bFreezedStop)
	{
		bUseControllerRotationYaw = false;
		return;
	}
	if (Value > 0.f)
	{
		MoveDirStat |= 8;
		MoveDirStat &= 11;
	}
	else if (Value < 0.f)
	{
		MoveDirStat |= 4;
		MoveDirStat &= 7;
	}
	else MoveDirStat &= 3;

	bool Tmp = (MoveDirStat == 0) || ((MoveDirStat & 1) && (MoveDirStat & 12));
	bUseControllerRotationYaw = !Tmp;
	
	if ( (Controller != NULL) && (Value != 0.0f) && GetCharacterMovement()->GetMaxSpeed() > 0.f )
	{
		if (GetMoveDirection() == EMoveDir::Backward)return;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

	////下面是为了动画过渡正常
	//if (bUseControllerRotationYaw && PreStat)
	//{
	//	bUseControllerRotationYaw = false;
	//	bCanUpdateControlYaw = false;
	//	
	//	FTimerDelegate TimerDelegate;
	//	TimerDelegate.BindLambda([&]() {
	//		if(!bInAbility)bUseControllerRotationYaw = true;
	//		bCanUpdateControlYaw = true;
	//		});
	//	GetWorldTimerManager().SetTimer(YawTimerHandle, TimerDelegate, 0.5f, false);
	//}
}

void AActionGameCharacter::AddControllerYawInput(float Val)
{
	
	YawSpeed = Val;
	Super::AddControllerYawInput(Val);
}

void AActionGameCharacter::AddControllerPitchInput(float Val)
{
	PitchSpeed = Val;
	Super::AddControllerPitchInput(Val);
}

EMoveDir::Type AActionGameCharacter::GetMoveDirection()
{
	if (MoveDirStat & 1)return EMoveDir::Forward;
	else if (MoveDirStat & 2)return EMoveDir::Backward;
	else if ((MoveDirStat & 4))return EMoveDir::Left;
	else return EMoveDir::Right;
}

void AActionGameCharacter::ApplyFreezedParticle(class UParticleSystem* InParticle)
{
	if (!InParticle)return;

	for (int32 i = 1; i <= 7; ++i)
	{
		const FString SocketName = TEXT("Freezed_") + FString::FormatAsNumber(i);
		UGameplayStatics::SpawnEmitterAttached(InParticle, GetMesh(), FName(*SocketName));
	}
}

void AActionGameCharacter::ApplyFreezedCameraParticle(class UParticleSystem* InParticle)
{
	
}

void AActionGameCharacter::EndFreezedSlow()
{
	bFreezedSlow = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AActionGameCharacter::EndFreezedStop()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->bNoSkeletonUpdate = false;
	GetMesh()->GetAnimInstance()->StopAllMontages(1.f);
	bFreezedStop = false;
	ResetCombo();
	bInAbility = false;
	if (AActionAIController* AIControl = Cast<AActionAIController>(GetController()))
		AIControl->SetAIFreezedValue();
}

bool AActionGameCharacter::HitReact(const FVector& HitPoint)
{
	if (bDead)return false;
	const FVector HitNormal = (HitPoint - GetActorLocation()).GetSafeNormal2D();
	const FVector PlayerDir = GetActorRotation().Vector();  //玩家方向
	const FVector PlayerRightDir = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);  //玩家右边的方向

	const float Degree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HitNormal, PlayerDir)));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Degree <= 45.f)
	{
		AnimInstance->Montage_Play(HitReactAnims[0], 1.f);
		return true;
	}
	
	if (Degree >= 135.f)
	{
		AnimInstance->Montage_Play(HitReactAnims[1], 1.f);
		return true;
	}
	else
	{
		if (FVector::DotProduct(PlayerRightDir, HitNormal) > 0.f)    //受击点在右方
			AnimInstance->Montage_Play(HitReactAnims[3], 1.f);
		else
			AnimInstance->Montage_Play(HitReactAnims[2], 1.f);
		return true;
	}
	return false;
}

float AActionGameCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= Damage;
	bTurboJumpAccelerate = false;

	if (Health <= 0.f)
	{
		bDead = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && DeathAnim)
			AnimInstance->Montage_Play(DeathAnim);
		if (DeadSound)     //播放死亡音效
			UGameplayStatics::PlaySoundAtLocation(this, DeadSound, GetActorLocation());

		AAGPlayerController* PlayerController = nullptr;
		//当前人物是AI
		if (auto AIController = Cast<AAIController>(GetController()))
		{
			if (AIController->GetPawn())
				AIController->UnPossess();
			PlayerController = Cast<AAGPlayerController>(EventInstigator);
			if(PlayerController)PlayerController->bIsWon = true;
		}
		else
		{
			if (EventInstigator->GetPawn())
				EventInstigator->UnPossess();
			PlayerController = Cast<AAGPlayerController>(GetController());
			if (PlayerController)
			{
				PlayerController->bIsWon = false;
				PlayerController->ConvertToDeathView();
				PlayerController->ApplyGrayScreen();
			}
		}

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, PlayerController, &AAGPlayerController::EndGame, 1.5f);
		Health = 0.f;
	}
	if (BlockSound)
		UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());
	OnHealthChanged.ExecuteIfBound(FMath::Abs(Health / GetClass()->GetDefaultObject<AActionGameCharacter>()->Health));
	return Health;
}
