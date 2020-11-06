// Fill out your copyright notice in the Description page of Project Settings.


#include "LeashEnemy.h"

ALeashEnemy::ALeashEnemy() : AEnemy()
{
	// Add callback to overlap event.
	OnActorBeginOverlap.AddDynamic(this, &ALeashEnemy::OnOverlapBegin);
}

void ALeashEnemy::Tick(float DeltaTime)
{
	if (EffectCooldown > 0.f)
	{
		EffectCooldown -= DeltaTime;
	}
}

void ALeashEnemy::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check for actor to leash.
	if (OtherActor && OtherActor != this)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player && EffectCooldown <= 0.f)
		{
			Player->Leash(FMath::RandRange(MIN_LEASH_DURATION, MAX_LEASH_DURATION), FMath::RandRange(MIN_HIT_COUNT, MAX_HIT_COUNT));
			EffectCooldown = DEFAULT_EFFECT_COOLDOWN;
		}
	}
}