// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckMatchLitDecorator.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"

UCheckMatchLitDecorator::UCheckMatchLitDecorator()
{
}

bool UCheckMatchLitDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	return Player->IsStealth();
}