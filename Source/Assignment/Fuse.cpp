// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuse.h"

// Sets default values
AFuse::AFuse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set torus as mesh.
	FuseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuseMesh"));
	RootComponent = FuseMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> FuseMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	if (FuseMeshAsset.Succeeded())
	{
		FuseMesh->SetStaticMesh(FuseMeshAsset.Object);
		FuseMesh->SetWorldScale3D(FVector(0.25f));
	}

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> FuseMaterial(TEXT("/Game/StarterContent/Materials/ItemMaterial.ItemMaterial"));
	if (FuseMaterial.Succeeded())
	{
		FuseMesh->SetMaterial(0, FuseMaterial.Object);
	}
}

// Called when the game starts or when spawned
void AFuse::BeginPlay()
{
	Super::BeginPlay();
	
	// Set up dynamic material instance.
	UMaterialInterface* Material = FuseMesh->GetMaterial(0);
	DynamicMaterialInstance = FuseMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor(1.f, 1.f, 1.f));
		DynamicMaterialInstance->SetVectorParameterValue("Colour 2", FLinearColor(.3f, .3f, 1.f));
	}
}

// Called every frame
void AFuse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFuse::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::GetEmpty();
	// Add item to inventory
	InteractingPlayer->AddItemToInventory(this);
	// Hide from world
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AFuse::Showcase_Implementation(FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("A fuse to activate some contraption...");
}
