// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimInstance_Aurora.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionAnimInstance_Aurora : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UActionAnimInstance_Aurora();

	UFUNCTION()
	void AnimNotify_CanStartAttack(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_SaveAttack(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_ResetCombo(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_StartMove(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_StopFly(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_EndQAbility(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_FreezeEnemy(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_EndRAbility(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_EndJumpMove(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_ToDeath(UAnimNotify* Notify);
};
