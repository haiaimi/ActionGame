// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameInstance.h"

UActionGameInstance::UActionGameInstance():
	PlayerIndex(0),
	PlayerSkinIndex(0)
{
	//FGCObject
	//FSlateDynamicImageBrush
}

void UActionGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

TSubclassOf<class AActionGameCharacter> UActionGameInstance::GetToSpawnPlayerClass()
{
	if (ToSpawnCharacter.Num() < 1)return nullptr;
	return ToSpawnCharacter[FMath::Clamp(PlayerSkinIndex, 0, ToSpawnCharacter.Num() - 1)];
}