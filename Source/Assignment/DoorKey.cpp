// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"

// Sets default values
ADoorKey::ADoorKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the key mesh to be a quad pyramid with rust material
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	RootComponent = KeyMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> KeyMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_QuadPyramid.Shape_QuadPyramid"));
	if (KeyMeshAsset.Succeeded())
	{
		KeyMesh->SetStaticMesh(KeyMeshAsset.Object);
		KeyMesh->SetWorldScale3D(FVector(0.25f));
	}

	ConstructorHelpers::FObjectFinder<UMaterial> MatchMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust"));
	if (MatchMaterial.Succeeded())
	{
		KeyMesh->SetMaterial(0, MatchMaterial.Object);
	}
}

// Called when the game starts or when spawned
void ADoorKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorKey::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::GetEmpty();
	// Add item to inventory
	InteractingPlayer->AddItemToInventory(this);
	// Hide from world
	SetActorHiddenInGame(true);
}

void ADoorKey::Showcase_Implementation(FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("A key that might come in handy for those pesky locked doors\nPress E to pick it up");
}