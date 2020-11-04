// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Turn rate of enemy
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 5.f, 0.f);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StunDuration > 0.f)
	{
		StunDuration -= DeltaTime;
		if (StunDuration <= 0.f)
		{
			ResetSpeed();
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when the enemy is stunned. Reduce move speed to 0.
void AEnemy::OnStunned(float NewStunDuration)
{
	StunDuration = NewStunDuration;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCharacterMovement()->MaxFlySpeed = 0.f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 0.f;
}

// Called when the enemy is no longer stunned. Reset all move speeds.
void AEnemy::ResetSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->MaxFlySpeed = 450.f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 450.f;
}
