// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimInstance_Countess.h"
#include "ActionGameCharacter_Countess.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAIAIMIHelper.h"
#include "TimerManager.h"

void UActionAnimInstance_Countess::AnimNotify_StopAttack(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Countess* CurOwner=Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->EnableAttack(false);
	}
}

void UActionAnimInstance_Countess::AnimNotify_SaveAttack(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Countess* CurOwner=Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->ComboAttackSave();
	}
}

void UActionAnimInstance_Countess::AnimNotify_ResetCombo(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Countess* CurOwner=Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->ResetCombo();
	}
}

void UActionAnimInstance_Countess::AnimNotify_RollingDarkSegment(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->SpawnRollingDarkSegemnts();
	}
}

void UActionAnimInstance_Countess::AnimNotify_AbilityFinished_Q(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->bInAbility = false;
	}
}

void UActionAnimInstance_Countess::AnimNotify_AbilityFinished_E(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->bInAbility = false;
	}
}

void UActionAnimInstance_Countess::AnimNotify_AbilityFinished_R(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->bInAbility = false;
	}
}

void UActionAnimInstance_Countess::AnimNotify_StartJump(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		//�����Ƕ�΢��
		if (AActionGameCharacter* Enemy = CurOwner->GetEnemy())
		{
			if (HAIAIMIHelper::GetDegreesBetweenActors(Enemy, CurOwner) < 45.f)
				CurOwner->bFaceToEnemy = true;
			CurOwner->bInvincible = true;
		}
	}
}

void UActionAnimInstance_Countess::AnimNotify_CatchEnemy(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->StabEnemy();
		CurOwner->bFaceToEnemy = false;
	}
}

void UActionAnimInstance_Countess::AnimNotify_LeaveEnemy(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->GetCharacterMovement()->JumpZVelocity = 600.f;
		CurOwner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		CurOwner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurOwner->SetActorRotation(FRotator(0.f, CurOwner->GetActorRotation().Yaw, 0.f));

		FVector CurLocation = CurOwner->GetActorLocation();
		CurLocation.Z = FMath::Clamp(CurLocation.Z, 120.f, 1000.f);
		CurOwner->SetActorLocation(CurLocation);
		CurOwner->bInvincible = false;
	}
}

void UActionAnimInstance_Countess::AnimNotify_ToDeath(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Countess* CurOwner = Cast<AActionGameCharacter_Countess>(Owner))
	{
		CurOwner->GetMesh()->bPauseAnims = true;
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, CurOwner]() {
			CurOwner->GetMesh()->SetSimulatePhysics(true);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, GetWorld()->DeltaTimeSeconds * 1.5f, false);
	}
}
