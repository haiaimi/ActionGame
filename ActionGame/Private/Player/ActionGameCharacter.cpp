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

//////////////////////////////////////////////////////////////////////////
// AActionGameCharacter

AActionGameCharacter::AActionGameCharacter():
	MoveDirStat(0),
	bInAbility(false),
	bFreezedSlow(false),
	bFreezedStop(false)
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

	SkillCoolingTimes.SetNum(3);
	SkillCoolingTimers.Init(FTimerHandle(), 3);
}

void AActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

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

	if (Controller)
		HAIAIMIHelper::Debug_ScreenMessage(Controller->GetName(),5.f);
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

void AActionGameCharacter::MakeAbilityCooling(int32 Index)
{
	if (Index >= SkillCoolingTimes.Num())return;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([Index, this]() {
		SetAbilityReady(Index);
		});
	GetWorldTimerManager().SetTimer(SkillCoolingTimers[Index], TimerDelegate, SkillCoolingTimes[Index], false);
	SkillCoolingTimes[Index] = 0.f;
	HAIAIMIHelper::Debug_ScreenMessage(FString::SanitizeFloat(GetWorldTimerManager().GetTimerRate(SkillCoolingTimers[Index])));
}

void AActionGameCharacter::SetAbilityReady(int32 Index)
{
	const float DefaultCoolingTime = GetClass()->GetDefaultObject<AActionGameCharacter>()->SkillCoolingTimes[Index];
	SkillCoolingTimes[Index] = DefaultCoolingTime;
}

void AActionGameCharacter::NormalAttack()
{
	
}

void AActionGameCharacter::Ability_Q()
{
	MakeAbilityCooling(0);
}

void AActionGameCharacter::Ability_E()
{
	MakeAbilityCooling(1);
}

void AActionGameCharacter::Ability_R()
{
	MakeAbilityCooling(2);
}

bool AActionGameCharacter::IsAbilityinCooling(int32 Index)
{
	return SkillCoolingTimes[Index] == 0.f;
}

float AActionGameCharacter::GetCoolingRate(int32 Index)
{
	const float DefaultCoolingTime = GetClass()->GetDefaultObject<AActionGameCharacter>()->SkillCoolingTimes[Index];
	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(SkillCoolingTimers[Index]);
	return 1 - RemainingTime / DefaultCoolingTime;
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
	//HAIAIMIHelper::Debug_ScreenMessage(FString::SanitizeFloat(Val));
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

bool AActionGameCharacter::HitReact(const FVector& HitPoint)
{
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
