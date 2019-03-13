// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_CastSkillQ.h"
#include "ActionAIController.h"
#include "ActionGameCharacter.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <GameFramework/CharacterMovementComponent.h>

EBTNodeResult::Type UBTTask_CastSkillQ::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	AActionGameCharacter* MyBot = Cast<AActionGameCharacter>(MyController->GetPawn());
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if(Enemy && MyBot)
	{
		if ((Enemy->GetActorLocation() - MyBot->GetActorLocation()).Size2D() > 500.f)
		{
			if (MyBot->GetCharacterMovement()->IsFalling() || MyBot->bInAbility || MyBot->bFreezedStop || MyBot->IsAbilityinCooling(EAbilityType::QAbility))return EBTNodeResult::Failed;
			MyBot->SetActorRotation((Enemy->GetActorLocation() - MyBot->GetActorLocation()).GetSafeNormal().ToOrientationRotator());
			MyBot->Ability_Q();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
