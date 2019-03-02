// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionGameInstance.h"

UActionGameInstance::UActionGameInstance():
	PlayerIndex(0),
	PlayerSkinIndex(0),
	EnemyIndex(0),
	EnemySkinIndex(0)
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
	PlayerIndex = FMath::Clamp(PlayerSkinIndex, 0, ToSpawnCharacter.Num() - 1);
	return ToSpawnCharacter[PlayerIndex];
}

TSubclassOf<class AActionGameCharacter> UActionGameInstance::GetToSpawnAIClass()
{
	if (ToSpawnAICharacter.Num() < 1)return nullptr;
	EnemyIndex = FMath::RandRange(0, ToSpawnAICharacter.Num() - 1);
	return ToSpawnAICharacter[EnemyIndex];
}
