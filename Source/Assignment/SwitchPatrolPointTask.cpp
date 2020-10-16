// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchPatrolPointTask.h"
#include "EnemyAIController.h"

// Task to rotate the patrol point of the fuse patroller.
EBTNodeResult::Type USwitchPatrolPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	MyController->SwitchPatrolPoint();

	return EBTNodeResult::Succeeded;
}
