// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimInstance_Countess.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionAnimInstance_Countess : public UAnimInstance
{
	GENERATED_BODY()
	
	UFUNCTION()
	void AnimNotify_SaveAttack(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_ResetCombo(UAnimNotify* Notify);
};
