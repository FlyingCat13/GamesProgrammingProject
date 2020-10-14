// Fill out your copyright notice in the Description page of Project Settings.


#include "Statue.h"

// Sets default values
AStatue::AStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh as that fancy Unreal Engine logo thing.
	StatueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatueMesh"));
	RootComponent = StatueMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> StatueMeshAsset(TEXT("/Game/StarterContent/Props/SM_MatPreviewMesh_02.SM_MatPreviewMesh_02"));
	if (StatueMeshAsset.Succeeded())
	{
		StatueMesh->SetStaticMesh(StatueMeshAsset.Object);
	}
	StatueMesh->SetWorldScale3D(FVector(.25f));
}

// Called when the game starts or when spawned
void AStatue::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInterface* Material = StatueMesh->GetMaterial(0);
	DynamicMaterialInstance = StatueMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMaterialInstance != nullptr)
	{
		float Glow = 1.f + FMath::Cos(GetWorld()->TimeSeconds * 2);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("Emissive"), Glow);
	}
}

// Add to inventory when interacted with.
void AStatue::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	Tooltip = FText::FromString("You got the relic! Now go back to your room before you go insane!");
	InteractingPlayer->AddItemToInventory(this);
	SetActorHiddenInGame(true);
}

// Description to the player
void AStatue::Showcase_Implementation(FText& Tooltip)
{
	Tooltip = FText::FromString("The prize of the night!\nPress E to pick it up");
}
