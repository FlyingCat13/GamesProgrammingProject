// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "LeashEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ALeashEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ALeashEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	const float DEFAULT_EFFECT_COOLDOWN = 10.f;
	const float MAX_LEASH_DURATION = 5.f;
	const float MIN_LEASH_DURATION = 3.f;
	const int MAX_HIT_COUNT = 8;
	const int MIN_HIT_COUNT = 4;

	float EffectCooldown = 0.f;

	// Callback for overlap event
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
