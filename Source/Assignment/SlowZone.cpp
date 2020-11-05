// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowZone.h"
#include "Components/BoxComponent.h"
#include "MainPlayer.h"

ASlowZone::ASlowZone()
{
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcessComponent->SetupAttachment(RootComponent);

	// Add callback to overlap begin event.
	OnActorBeginOverlap.AddDynamic(this, &ASlowZone::OnOverlapBegin);
	// Add callback to overlap event.
	OnActorEndOverlap.AddDynamic(this, &ASlowZone::OnOverlapEnd);
}

void ASlowZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player)
		{
			Player->SetRealSpeed(SLOW_SPEED);
		}
	}
}

void ASlowZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player)
		{
			Player->SetRealSpeed(DEFAULT_SPEED);
		}
	}
}