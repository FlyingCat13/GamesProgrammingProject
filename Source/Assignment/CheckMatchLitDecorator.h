// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CheckMatchLitDecorator.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UCheckMatchLitDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCheckMatchLitDecorator();

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
