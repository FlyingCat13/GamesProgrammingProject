// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "DoorTrigger.generated.h"

// Door unlock event.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnlockDoor);

UCLASS()
class ASSIGNMENT_API ADoorTrigger : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorTrigger();

	// Mesh of the item
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BaseMesh;

	// Functions from Interactable interface.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;

	// Door unlock event to broadcast.
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FOnUnlockDoor OnUnlockDoorEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
