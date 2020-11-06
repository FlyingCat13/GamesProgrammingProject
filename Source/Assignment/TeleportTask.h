// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeleportTask.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UTeleportTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
