// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGameGameMode.h"
#include "ActionGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ActionGameInstance.h"
#include "HAIAIMIHelper.h"

AActionGameGameMode::AActionGameGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

void AActionGameGameMode::RestartPlayer(AController* NewPlayer)
{
	//从GameInstance获取生成对象信息
	if (UActionGameInstance* MyInstance = GetGameInstance<UActionGameInstance>())
	{
		DefaultPawnClass = MyInstance->GetToSpawnPlayerClass();
	}

	Super::RestartPlayer(NewPlayer);
}

void AActionGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
