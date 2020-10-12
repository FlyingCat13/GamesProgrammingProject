// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Consumable.h"
#include "Interactable.h"
#include "Inventoriable.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 70.f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// Override post processing settings for the camera effects.
	FirstPersonCameraComponent->PostProcessSettings.bOverride_AutoExposureBias = true;
	FirstPersonCameraComponent->PostProcessSettings.bOverride_SceneColorTint = true;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<AAssignmentGameMode>(GetWorld()->GetAuthGameMode());
	SetDefaultCameraEffect();

	GameMode->OnUpdateInventoryStatus(InventoryInText(), SelectedInventorySlot + 1);
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CallMyTrace(false);

	GameMode->OnUpdateHealthBar(GetHealthPercentage());

	float OldDebuffCountdown = DebuffCountdown;
	
	// Prioritise decressing match timer first before normal timer.
	if (MatchCountdown > 0.f)
	{
		MatchCountdown -= DeltaTime;
		// If match runs out, switch to normal if debuff countdown is still positive 
		// or switch to Cursed if otherwise.
		if (MatchCountdown < 0.f)
		{
			if (DebuffCountdown > 0.f)
			{
				// Switch back to default camera effect
				SetDefaultCameraEffect();
				// DebuffCountdown absorb the negative value of MatchCountdown
				DebuffCountdown += MatchCountdown;
			}
			else
			{
				// Switch back to debuff camera effect
				SetDebuffCameraEffect();
			}
		}
	}
	else if (DebuffCountdown > 0.f)
	{
		DebuffCountdown -= DeltaTime;
	}
	
	// DebuffCountdown < 0.f implies debuff in effect.
	if (DebuffCountdown < 0.f)
	{	
		float DebuffDeltaTime = DeltaTime;
		// If switching to debuff mode, switch the camera effect and offset the
		// negative time to calculate health decay value.
		if (OldDebuffCountdown > 0.f)
		{
			SetDebuffCameraEffect();
			DebuffDeltaTime = -DebuffCountdown;
			// Give DebuffCountdown a negative value so this if branch will not be triggered
			// in the future unless DebuffCountdown is set to positive later.
			DebuffCountdown = -1.f;
		}

		// Decrease player's health with rate coinciding with delta time.
		if (MatchCountdown <= 0.f)
		{
			Health -= BASE_DEBUFF_DECAY_RATE * DebuffDeltaTime;
		}
	}

	if (Health <= 0.f)
	{
		//GameMode->OnLose();
		// Broadcast the player die event.
		OnPlayerDieEvent.Broadcast();
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	// Bind turn events
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Bind interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayer::Interact);

	// Bind switch inventory event
	PlayerInputComponent->BindAction("SwitchInventory", IE_Pressed, this, &AMainPlayer::SwitchInventory);

	// Bind pause event
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainPlayer::Pause).bExecuteWhenPaused = true;
}

void AMainPlayer::AddItemToInventory(AActor* ItemToAdd)
{
	// Only add if the item is Inventoriable.
	if (ItemToAdd->GetClass()->ImplementsInterface(UInventoriable::StaticClass())) {
		Inventory.Add(ItemToAdd);
	}
	GameMode->OnUpdateInventoryStatus(InventoryInText(), SelectedInventorySlot + 1);
}

void AMainPlayer::RemoveItemFromInventory(AActor* ItemToRemove)
{
	// Remove item and adjust current selected item if needed.
	Inventory.Remove(ItemToRemove);
	if (SelectedInventorySlot >= Inventory.Num())
	{
		SelectedInventorySlot = -1;
	}
	GameMode->OnUpdateInventoryStatus(InventoryInText(), SelectedInventorySlot + 1);
}

AActor* AMainPlayer::GetEquippedItem()
{	
	// Get currently equipped item. Return nullptr if not equipping anything.
	if (SelectedInventorySlot > -1)
	{
		return Inventory[SelectedInventorySlot];
	}
	else
	{
		return nullptr;
	}
}

// Returns inventory as an array of FText.
TArray<FText> AMainPlayer::InventoryInText()
{
	TArray<FText> Result;

	Result.Add(FText::FromString("None"));

	for (auto& Item : Inventory)
	{
		Result.Add(FText::FromString(Item->GetName()));
	}

	return Result;
}

bool AMainPlayer::IsInInventory(AActor* ItemToCheck)
{
	// Find item in inventory.
	return Inventory.Find(ItemToCheck) != INDEX_NONE;
}

void AMainPlayer::MoveForward(float Value)
{
	if (Value != .0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMainPlayer::MoveRight(float Value)
{
	if (Value != .0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMainPlayer::Interact()
{	
	// Interact with items by trace if not holding consumable. Otherwise consume the equipped item.
	if (SelectedInventorySlot > -1 && Inventory[SelectedInventorySlot]->GetClass()->ImplementsInterface(UConsumable::StaticClass()))
	{
		IConsumable::Execute_OnConsume(Inventory[SelectedInventorySlot], this);
	}
	else 
	{
		CallMyTrace(true);
	}
}

// The following functions are taken from the example Moodle code with modification to fit the project.
// Trace functions - used to detect items we are looking at in the world

// Trace() - called by our CallMyTrace() function which sets up our parameters and passes them through
bool AMainPlayer::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false
) {

	// The World parameter refers to our game world (map/level) 
	// If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	//const FName TraceTag("MyTraceTag");
	//World->DebugDrawTraceTag = TraceTag;
	//TraceParams.TraceTag = TraceTag;


	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel
	(
		HitOut,		//result
		Start,	//start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}

// CallMyTrace() - sets up our parameters and then calls our Trace() function
void AMainPlayer::CallMyTrace(bool IsInteracting)
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * ReachLength;

	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);

	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;

	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor())
		{

			UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
			ProcessTraceHit(HitData, IsInteracting);

		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
		//ClearPickupInfo();
		GameMode->OnLookAtItemTooltipUpdate(FText::GetEmpty());
	}

}

// ProcessTraceHit() - process our Trace Hit result
void AMainPlayer::ProcessTraceHit(FHitResult& HitOut, bool IsInteracting)
{
	if (HitOut.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		// Depending on the type of trace (just looking at the item or interacting), trigger the appropriate function
		// while also updating the appropriate tooltip
		FText Tooltip;
		if (IsInteracting)
		{
			IInteractable::Execute_Interact(HitOut.GetActor(), this, Tooltip);
			GameMode->OnInteractedItemTooltipUpdate(Tooltip);
		}
		else
		{
			IInteractable::Execute_Showcase(HitOut.GetActor(), Tooltip);
			GameMode->OnLookAtItemTooltipUpdate(Tooltip);
		}
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("Actor is NOT Interactable!"));
		//ClearPickupInfo();
	}
}

void AMainPlayer::Heal(float HealAmount)
{
	// Heal up to MAX_HEALTH.
	Health = FMath::Min(MAX_HEALTH, Health + HealAmount);
}

void AMainPlayer::ResetDebuff()
{
	// Reset debuff countdown.
	DebuffCountdown = BASE_DEBUFF_COUNTDOWN;
	// Switch to default camera effect if match light is not at play.
	if (MatchCountdown <= 0.f)
	{
		SetDefaultCameraEffect();
	}
}

void AMainPlayer::LightMatch(float MatchDuration)
{
	// Activate match by setting match duration.
	MatchCountdown = MatchDuration;
	// Switch to match light camera effect.
	SetMatchLightCameraEffect();
}

float AMainPlayer::GetHealthPercentage()
{
	return Health / MAX_HEALTH;
}

void AMainPlayer::SetReachLength(int NewReachLength)
{
	// Set reach length (trace length) to new value.
	ReachLength = NewReachLength;
}

void AMainPlayer::ResetReachLength()
{
	// Reset reach length to default.
	ReachLength = BASE_REACH_LENGTH;
}

void AMainPlayer::SetDefaultCameraEffect()
{
	// Dark normal colour.
	FirstPersonCameraComponent->PostProcessSettings.AutoExposureBias = -.25f;
	FirstPersonCameraComponent->PostProcessSettings.SceneColorTint = FLinearColor(1.f, 1.f, 1.f);
}

void AMainPlayer::SetDebuffCameraEffect()
{
	// Slightly brighter but red tint.
	FirstPersonCameraComponent->PostProcessSettings.AutoExposureBias = 1.f;
	FirstPersonCameraComponent->PostProcessSettings.SceneColorTint = FLinearColor(1.f, 0.f, 0.f);
}

void AMainPlayer::SetMatchLightCameraEffect()
{
	// Very bright with warm tint.
	FirstPersonCameraComponent->PostProcessSettings.AutoExposureBias = 3.f;
	FirstPersonCameraComponent->PostProcessSettings.SceneColorTint = FLinearColor(1.f, .6f, 0.f);
}

void AMainPlayer::SwitchInventory()
{
	// Cycle selected slot around -1 to Inventory.Num() - 1 while executing unequip of old item
	// and equip of new.
	if (SelectedInventorySlot > -1)
	{
		IInventoriable::Execute_OnUnequip(Inventory[SelectedInventorySlot], this);
	}
	SelectedInventorySlot = (SelectedInventorySlot + 2) % (Inventory.Num() + 1) - 1;
	if (SelectedInventorySlot > -1)
	{
		IInventoriable::Execute_OnEquip(Inventory[SelectedInventorySlot], this);
	}
	GameMode->OnUpdateInventoryStatus(InventoryInText(), SelectedInventorySlot + 1);
}

void AMainPlayer::Pause()
{
	GameMode->OnPressPause();
}