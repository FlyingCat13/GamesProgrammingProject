// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"

// Sets default values
AInteractableDoor::AInteractableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = DoorFrame;
	ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameMesh(TEXT("/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame"));
	if (DoorFrameMesh.Succeeded())
	{
		DoorFrame->SetStaticMesh(DoorFrameMesh.Object);
	}

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
	
}

// Called every frame
void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableDoor::Interact_Implementation() {
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
