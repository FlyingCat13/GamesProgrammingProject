// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Inventoriable.h"
#include "GameFramework/Actor.h"
#include "DoorKey.generated.h"

UCLASS()
class ASSIGNMENT_API ADoorKey : public AActor, public IInteractable, public IInventoriable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorKey();

	// Mesh of the item
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* KeyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions from Interactable interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;
};
