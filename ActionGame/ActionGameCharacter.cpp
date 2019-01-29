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
#include <Animation/AnimInstance.h>
#include "AI/ActionAIController.h"

//////////////////////////////////////////////////////////////////////////
// AActionGameCharacter

AActionGameCharacter::AActionGameCharacter():
	MoveDirStat(0),
	bInAbility(false),
	bFreezedSlow(false)
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
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AActionGameCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AActionGameCharacter::NormalAttack()
{

}

void AActionGameCharacter::Ability_Q()
{

}

void AActionGameCharacter::Ability_E()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Ability E"));
}

void AActionGameCharacter::Ability_R()
{

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
	//if (Controller->IsA(AActionAIController::StaticClass()))
	if (Value > 0.f)MoveDirStat |= 1;
	if (Value < 0.f)MoveDirStat |= 2;
	if (Value == 0.f)MoveDirStat &= 12;

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
	const bool PreStat = (MoveDirStat & 1) && (MoveDirStat & 12);
	if (Value > 0.f)MoveDirStat |= 8;
	if (Value < 0.f)MoveDirStat |= 4;
	if (Value == 0.f)MoveDirStat &= 3;

	bool Tmp = (MoveDirStat == 0) || ((MoveDirStat & 1) && (MoveDirStat & 12));
	bUseControllerRotationYaw = !Tmp;

	//if (Controller->IsA(AActionAIController::StaticClass()))
	
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
