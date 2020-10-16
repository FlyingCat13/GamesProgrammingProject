// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Switch.generated.h"

UCLASS()
class ASSIGNMENT_API ASwitch : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* SwitchMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool Value = false;
	// This manages whether to prevent the player from interacting with the switch (i.e. when the panel
	// is already activated).
	bool IsDisabled = false;

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

	// Accessors and mutators
	bool GetValue();
	void SetValue(bool NewValue);
	void SetIsDisabled(bool NewStatus);
};
