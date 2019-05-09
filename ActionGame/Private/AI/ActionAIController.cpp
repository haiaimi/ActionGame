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
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameBotInterface.h"

AActionAIController::AActionAIController():
	bInSurround(false),
	Enemy(nullptr)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));	

	bWantsPlayerState = true;
	
}

void AActionAIController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
}

void AActionAIController::GameHasEnded(class AActor* EndGameFocus /*= NULL*/, bool bIsWinner /*= false*/)
{
	Super::GameHasEnded();
}

void AActionAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActionAIController::StartAIPlayer, 4.f);
}

void AActionAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BehaviorComp->StopTree();
}

void AActionAIController::BeginInactiveState()
{
	Super::BeginInactiveState();
}

class AActionGameCharacter* AActionAIController::GetEnemy()
{
	if (Enemy)
	{
		//HAIAIMIHelper::Debug_ScreenMessage(Enemy->GetName());
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, Enemy);
		return Enemy;
	}
	
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		if (It->Get() != GetPawn() && It->Get()->AIControllerClass != AActionAIController::StaticClass())
		{
			BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, It->Get());
			Enemy = Cast<AActionGameCharacter>(It->Get());
		
			return Enemy;
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
	UObject* BTEnemy = BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID);
	AActionGameCharacter* AIPlayer = Cast<AActionGameCharacter>(GetPawn());

	bool Result = AIPlayer->bFreezedSlow;
	
	if (AActionGameCharacter* Enemy_A = Cast<AActionGameCharacter>(BTEnemy))
	{
		if (Enemy_A->bFreezedSlow || Enemy_A->bFreezedStop || AIPlayer->bInAbility)  //敌人也处于冰冻减速状态，就硬杠
			Result = false;
		else
			Result = Result || Enemy_A->IsInAbility(EAbilityType::RAbility);
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

bool AActionAIController::HasStarted()
{
	return BehaviorComp->IsRunning();
}

void AActionAIController::StartAIPlayer()
{
	if (BehaviorComp->IsRunning())return;
	IGameBotInterface* Bot = Cast<IGameBotInterface>(GetPawn());
	ensure(Bot);
	if (Bot && Bot->BehaviorTree)
	{
		if (Bot->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BehaviorTree->BlackboardAsset);
		}
		EnemyKeyID = BlackboardComp->GetKeyID(TEXT("Enemy"));
		FreezedKeyID = BlackboardComp->GetKeyID(TEXT("bFreezed"));
		MoveBackKeyID = BlackboardComp->GetKeyID(TEXT("bMoveBack"));
		BlackboardComp->SetValueAsBool(TEXT("bInSurround"), false);
	}

	BehaviorComp->StartTree(*(Bot->BehaviorTree));

	if (AActionGameCharacter* MEnemy = GetEnemy())
	{
		APlayerController* MC = MEnemy->GetController<APlayerController>();
		MEnemy->EnableInput(MC);
	}
}

void AActionAIController::SetSurroundState(bool InState)
{
	bInSurround = InState;
	BlackboardComp->SetValueAsBool(TEXT("bInSurround"), bInSurround);
}
