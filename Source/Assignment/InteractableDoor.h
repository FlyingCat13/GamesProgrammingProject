// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableDoor.generated.h"

UCLASS()
class ASSIGNMENT_API AInteractableDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableDoor();

	// Door frame mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* DoorFrame;

	// Door mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Door;

	// Interactable functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;

	// Open/close the door
	void ToggleOpen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsOpened = false;

	// Dynamic material for glowing effects.
	UMaterialInstanceDynamic* DoorFrameMaterialInstance;
	UMaterialInstanceDynamic* DoorMaterialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
