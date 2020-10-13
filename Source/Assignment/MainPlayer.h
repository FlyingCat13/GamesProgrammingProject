// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssignmentGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

// Player die event (health drops to 0 or lower).
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDie);

UCLASS()
class ASSIGNMENT_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

	// First person camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// Player die event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FOnPlayerDie OnPlayerDieEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Inventory related functions
	void AddItemToInventory(AActor* ItemToAdd);
	void RemoveItemFromInventory(AActor* ItemToRemove);
	AActor* GetEquippedItem();
	bool IsInInventory(AActor* ItemToCheck);
	TArray<FText> InventoryInText();
	int GetSelectedInventorySlot();

	// Health and debuff related functions
	void Heal(float HealAmount);
	void ResetDebuff();
	void LightMatch(float MatchDuration);
	float GetHealthPercentage();

	// Interaction reach length related functions
	void SetReachLength(int NewReachLength);
	void ResetReachLength();

protected:
	// Utility constant
	const float MAX_HEALTH = 100.f;
	const float BASE_DEBUFF_COUNTDOWN = 3.f;
	const float BASE_DEBUFF_DECAY_RATE = 2.f;
	const int BASE_REACH_LENGTH = 150;

	// Reference to game mode to communicate
	AAssignmentGameMode* GameMode;

	// Currently equipped item. -1 being equipping nothing.
	int SelectedInventorySlot = -1;
	// Item interaction reach length.
	int ReachLength = BASE_REACH_LENGTH;

	// Inventory
	TArray<AActor*> Inventory;

	// Health variable
	float Health = MAX_HEALTH;
	// Countdown until health draining occurs
	float DebuffCountdown = BASE_DEBUFF_COUNTDOWN;
	// Countdown until match runs out of light.
	float MatchCountdown = 0.f;

	// Handles moving forward/backward
	void MoveForward(float Val);

	// Handles stafing movement, left and right
	void MoveRight(float Val);

	// Handles item interactions
	void Interact();

	// Handles cycling through inventory
	void SwitchInventory();

	// Handles pausing the game
	void Pause();

	// Trace functions from the lecture with slight modification in reach length and interactions.
	bool Trace(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel,
		bool ReturnPhysMat
	);
	// Call trace can trigger interactions or tooltip if the player is only looking at it.
	void CallMyTrace(bool IsInteracting = false);
	void ProcessTraceHit(FHitResult& HitOut, bool IsInteracting);

	// Different camera effects for different states.
	void SetDefaultCameraEffect();
	void SetDebuffCameraEffect();
	void SetMatchLightCameraEffect();
};
