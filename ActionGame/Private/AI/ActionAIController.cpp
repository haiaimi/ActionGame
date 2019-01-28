// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ActionAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "AI/ActionGameBot_Aurora.h"
#include <BehaviorTree/BehaviorTree.h>
#include "ActionGameCharacter.h"
#include <EngineUtils.h>
#include "Common/HAIAIMIHelper.h"

AActionAIController::AActionAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));	

	bWantsPlayerState = true;
}

void AActionAIController::GameHasEnded(class AActor* EndGameFocus /*= NULL*/, bool bIsWinner /*= false*/)
{
	Super::GameHasEnded();
}

void AActionAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	class AActionGameBot_Aurora* Bot = Cast<AActionGameBot_Aurora>(InPawn);
	if(Bot && Bot->BotBehavior)
	{
		if(Bot->BotBehavior->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}
	}

	HAIAIMIHelper::Debug_ScreenMessage(TEXT("Possess Pawn"));
	BehaviorComp->StartTree(*(Bot->BotBehavior));
}

void AActionAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AActionAIController::BeginInactiveState()
{
	Super::BeginInactiveState();
}

class AActionGameCharacter* AActionAIController::GetEnemy()
{
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		if (It->Get() != GetPawn())
			return Cast<AActionGameCharacter>(It->Get());
	}
	return nullptr;
}
