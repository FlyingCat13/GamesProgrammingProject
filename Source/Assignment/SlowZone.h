// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PostProcessComponent.h"
#include "Engine/TriggerBox.h"
#include "SlowZone.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ASlowZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASlowZone();

	// Callback for overlap event begin
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	// Callback for overlap event end.
	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPostProcessComponent* PostProcessComponent;

protected:
	const float SLOW_SPEED = 100.f;
	const float DEFAULT_SPEED = 600.f;
};
