// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameType.generated.h"

#define COLLISION_ICETRACE ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EMoveDir
{
	enum Type
	{
		Forward,      
		Backward,
		Left,
		Right
	};
}

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText CharName;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> CharMeshs;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> AbilityAnims;

	UPROPERTY(EditDefaultsOnly)
	TArray<FText> MeshNames;
};