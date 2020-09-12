// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "MainPlayer.h"
#include "Candy.generated.h"

// Healing item that also dispel the health drain effect for a while.
UCLASS()
class ASSIGNMENT_API ACandy : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACandy();

	// Mesh of the item.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* CandyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Amount of heal.
	const float HEAL_AMOUNT = 50.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions from Interactable interface.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;
};
