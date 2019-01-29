// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimInstance_Aurora.h"
#include "ActionGameCharacter_Aurora.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <Particles/ParticleSystem.h>

UActionAnimInstance_Aurora::UActionAnimInstance_Aurora()
{
	
}

void UActionAnimInstance_Aurora::AnimNotify_CanStartAttack(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->PlayerCanAttack();
	}
}

void UActionAnimInstance_Aurora::AnimNotify_SaveAttack(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->ComboAttackSave();
	}
}

void UActionAnimInstance_Aurora::AnimNotify_ResetCombo(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->ResetCombo();
	}
}

void UActionAnimInstance_Aurora::AnimNotify_StartMove(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurOwner->bInAbility = false;
	}
}

void UActionAnimInstance_Aurora::AnimNotify_StopFly(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->bTurboJumpAccelerate = false;
	}
}

void UActionAnimInstance_Aurora::AnimNotify_EndQAbility(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if(AActionGameCharacter_Aurora* CurOwner=Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->bTurboJumpAccelerate = false;
		CurOwner->bInAbility = false;
	}
}

void UActionAnimInstance_Aurora::AnimNotify_RAbilityWarmUp(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Aurora* CurOwner = Cast<AActionGameCharacter_Aurora>(Owner))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WarmUpParticle, CurOwner->GetActorTransform());
	}
}

void UActionAnimInstance_Aurora::AnimNotify_RAbilityBlast(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Aurora* CurOwner = Cast<AActionGameCharacter_Aurora>(Owner))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlastParticle, CurOwner->GetActorTransform());
	}
}

void UActionAnimInstance_Aurora::AnimNotify_FreezeEnemy(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Aurora* CurOwner = Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->FreezeEnemy();
	}
}

void UActionAnimInstance_Aurora::AnimNotify_EndRAbility(UAnimNotify* Notify)
{
	APawn* Owner = TryGetPawnOwner();
	if (AActionGameCharacter_Aurora* CurOwner = Cast<AActionGameCharacter_Aurora>(Owner))
	{
		CurOwner->GetCharacterMovement()->JumpZVelocity = 600.f;    //恢复人物跳跃高度
		CurOwner->bInAbility = false;
	}
}
