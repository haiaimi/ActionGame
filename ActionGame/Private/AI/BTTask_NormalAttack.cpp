// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_NormalAttack.h"
#include "AI/ActionAIController.h"
#include <NavigationSystem.h>
#include "ActionGameCharacter.h"
#include "Common/HAIAIMIHelper.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	AActionGameCharacter* MyBot = Cast<AActionGameCharacter>(MyController->GetPawn());
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if (!MyBot->bFreezedSlow && !MyBot->bFreezedStop)
		MyBot->GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if(Enemy && MyBot && !Enemy->IsDead())
	{
		if ((Enemy->GetActorLocation() - MyBot->GetActorLocation()).Size2D() < 250.f)
		{
			MyBot->NormalAttack();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Succeeded;
}
