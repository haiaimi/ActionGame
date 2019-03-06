// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGameGameMode.h"
#include "ActionGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ActionGameInstance.h"
#include "HAIAIMIHelper.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameHUD.h"

AActionGameGameMode::AActionGameGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	HUDClass = AGameHUD::StaticClass();
}

void AActionGameGameMode::RestartPlayer(AController* NewPlayer)
{
	//从GameInstance获取生成对象信息
	if (UActionGameInstance* MyInstance = GetGameInstance<UActionGameInstance>())
	{
		DefaultPawnClass = MyInstance->GetToSpawnPlayerClass();

		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			APlayerStart* Start = *It;
			if (Start && Start->PlayerStartTag == FName(TEXT("Enemy")))
			{
				if (MyInstance->GetToSpawnAIClass())
					GetWorld()->SpawnActor<AActionGameCharacter>(MyInstance->GetToSpawnAIClass(), Start->GetActorTransform());
				else
					HAIAIMIHelper::Debug_LogMessage(TEXT("Spawn Enemy Failed"));
			}
		}
	}

	Super::RestartPlayer(NewPlayer);
}

APlayerController* AActionGameGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, TEXT("Player"), Options, UniqueId, ErrorMessage);
}

void AActionGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
