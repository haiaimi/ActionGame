// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimInstance_Countess.h"
#include "ActionGameCharacter_Countess.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAIAIMIHelper.h"

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
		//¹¥»÷½Ç¶ÈÎ¢µ÷
		for (TActorIterator<AActionGameCharacter> Iter(GetWorld()); Iter; ++Iter)
		{
			if (*Iter == CurOwner)continue;
			const FVector ToEnemyDir = ((*Iter)->GetActorLocation() - CurOwner->GetActorLocation()).GetSafeNormal2D();
			const FVector ForwardDir = CurOwner->GetActorRotation().Vector();
			const float SubDegree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToEnemyDir, ForwardDir)));

			if (SubDegree < 45.f)
			{
				CurOwner->bFaceToEnemy = true;
				CurOwner->Enemy = *Iter;
			}
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
	}
}
