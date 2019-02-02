// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/ActionAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "AI/ActionGameBot_Aurora.h"
#include <BehaviorTree/BehaviorTree.h>
#include "ActionGameCharacter.h"
#include <EngineUtils.h>
#include "Common/HAIAIMIHelper.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Bool.h>
#include "GameFramework/CharacterMovementComponent.h"

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
		EnemyKeyID = BlackboardComp->GetKeyID(TEXT("Enemy"));
		FreezedKeyID = BlackboardComp->GetKeyID(TEXT("bFreezed"));
		MoveBackKeyID = BlackboardComp->GetKeyID(TEXT("bMoveBack"));
	}

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
		{
			BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, It->Get());
			return Cast<AActionGameCharacter>(It->Get());
		}	
	}
	return nullptr;
}

void AActionAIController::SetAIFreezedValue()
{
	AActionGameCharacter* AIPlayer = Cast<AActionGameCharacter>(GetPawn());
	BlackboardComp->SetValue<UBlackboardKeyType_Bool>(FreezedKeyID, AIPlayer->bFreezedStop);
}

bool AActionAIController::ShouldMoveBack()
{
	UObject* Enemy = BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID);
	AActionGameCharacter* AIPlayer = Cast<AActionGameCharacter>(GetPawn());

	bool Result = AIPlayer->bFreezedSlow;
	
	if (AActionGameCharacter_Aurora* Enemy_A = Cast<AActionGameCharacter_Aurora>(Enemy))
	{
		if (Enemy_A->bFreezedSlow || Enemy_A->bFreezedStop || AIPlayer->bInAbility)  //敌人也处于冰冻减速状态，就硬杠
			Result = false;
		else
			Result = Result || Enemy_A->IsCastAbility_R();
	}
	BlackboardComp->SetValue<UBlackboardKeyType_Bool>(MoveBackKeyID, Result);
	AIPlayer->GetCharacterMovement()->bOrientRotationToMovement = !Result;
	AIPlayer->SetMoveDir(Result ? EMoveDir::Backward : EMoveDir::Forward);

	return Result;
}

bool AActionAIController::IsMovingBack()
{
	return BlackboardComp->GetValue<UBlackboardKeyType_Bool>(MoveBackKeyID);
}
