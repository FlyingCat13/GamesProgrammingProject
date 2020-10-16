// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Inventoriable.h"
#include "MainPlayer.h"
#include "GameFramework/Actor.h"
#include "Stick.generated.h"

UCLASS()
class ASSIGNMENT_API AStick : public AActor, public IInteractable, public IInventoriable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStick();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* StickMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	const int REACH_LENGTH = 500;

	// Animated dynamic material.
	UMaterialInstanceDynamic* DynamicMaterialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interactable functions 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;

	// Inventoriable functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void OnEquip(AMainPlayer* InteractingPlayer);
	virtual void OnEquip_Implementation(AMainPlayer* InteractingPlayer) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void OnUnequip(AMainPlayer* InteractingPlayer);
	virtual void OnUnequip_Implementation(AMainPlayer* InteractingPlayer) override;


};
