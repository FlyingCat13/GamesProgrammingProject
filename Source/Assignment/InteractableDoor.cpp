// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

// Sets default values
AInteractableDoor::AInteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up door frame mesh (also the root component)
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = DoorFrame;
	ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameMesh(TEXT("/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame"));
	if (DoorFrameMesh.Succeeded())
	{
		DoorFrame->SetStaticMesh(DoorFrameMesh.Object);
	}

	// Set up the door mesh.
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMesh(TEXT("/Game/StarterContent/Props/SM_Door.SM_Door"));
	if (DoorMesh.Succeeded())
	{
		Door->SetStaticMesh(DoorMesh.Object);
	}

	Door->SetRelativeLocation(FVector(0.f, 45.f, 0.f));
	DoorFrame->SetWorldScale3D(FVector(1.f, 1.1f, 1.f));
}

// Called when the game starts or when spawned
void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInterface* DoorFrameMaterial = DoorFrame->GetMaterial(0);
	DoorFrameMaterialInstance = DoorFrame->CreateDynamicMaterialInstance(0, DoorFrameMaterial);
	UMaterialInterface* DoorMaterial = Door->GetMaterial(0);
	DoorMaterialInstance = Door->CreateDynamicMaterialInstance(0, DoorMaterial);

	if (DoorFrameMaterialInstance && DoorMaterialInstance)
	{
		DoorFrameMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
		DoorFrameMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);

		DoorMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
		DoorMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);
	}
}

// Called every frame
void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Toggle open upon interaction.
void AInteractableDoor::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	Tooltip = FText::GetEmpty();
	ToggleOpen();
}

// Show player they can press E to open/close
void AInteractableDoor::Showcase_Implementation(FText& Tooltip)
{
	if (!IsOpened)
	{
		Tooltip = FText::FromString("Press E to open");
	}
	else
	{
		Tooltip = FText::FromString("Press E to close");
	}
}

// Spin the door mesh 90 degrees to open/close
void AInteractableDoor::ToggleOpen()
{
	IsOpened = !IsOpened;
	if (IsOpened)
	{
		Door->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
	else
	{
		Door->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
}