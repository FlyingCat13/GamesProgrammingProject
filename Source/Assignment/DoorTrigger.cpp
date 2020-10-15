// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTrigger.h"
#include "LockableDoor.h"

// Sets default values
ADoorTrigger::ADoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set only an empty mesh as the root component to allow for future customisability
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Game/StarterContent/Materials/ItemMaterial.ItemMaterial"));
	if (BaseMaterial.Succeeded())
	{
		BaseMesh->SetMaterial(0, BaseMaterial.Object);
	}
}

// Called when the game starts or when spawned
void ADoorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	// Set up dynamic material instance.
	UMaterialInterface* Material = BaseMesh->GetMaterial(0);
	DynamicMaterialInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor::Red);
	}
}

// Called every frame
void ADoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorTrigger::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("A door has been unlocked");
	// Unlock the designated door by broadcasting an event.
	//TargetDoor->Unlock();
	OnUnlockDoorEvent.Broadcast();
	// Change colours when triggered.
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor::Green);
	}
}

void ADoorTrigger::Showcase_Implementation(FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("Hmm this might be interesting\nPress E to interact");
}