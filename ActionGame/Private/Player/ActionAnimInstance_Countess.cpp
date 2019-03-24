// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimInstance_Countess.h"
#include "ActionGameCharacter_Countess.h"

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
