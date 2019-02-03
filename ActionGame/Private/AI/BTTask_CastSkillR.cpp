// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_CastSkillR.h"
#include "ActionAIController.h"
#include "ActionGameCharacter.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <GameFramework/CharacterMovementComponent.h>

EBTNodeResult::Type UBTTask_CastSkillR::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr || MyController->IsMovingBack())
		return EBTNodeResult::Failed;

	AActionGameCharacter* MyBot = Cast<AActionGameCharacter>(MyController->GetPawn());
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if(Enemy && MyBot)
	{
		if ((Enemy->GetActorLocation() - MyBot->GetActorLocation()).Size2D() < 400.f)
		{
			if (MyBot->IsAbilityinCooling(2) || MyBot->bInAbility || MyBot->bFreezedSlow || MyBot->bFreezedStop)
				return EBTNodeResult::Failed;

			MyBot->Ability_R();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
