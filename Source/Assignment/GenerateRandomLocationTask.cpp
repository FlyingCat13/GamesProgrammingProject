// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateRandomLocationTask.h"
#include "EnemyAIController.h"

// Behavior tree task to call the random location function in the controller.
EBTNodeResult::Type UGenerateRandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());

	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}

	MyController->GenerateNewRandomLocation();

	return EBTNodeResult::Succeeded;
}