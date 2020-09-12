// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	// Expose buttons publicly to hook callbacks to it in Game Mode.
	UPROPERTY(EditAnywhere)
		class UButton* StartButton;

	UPROPERTY(EditAnywhere)
		class UButton* QuitButton;
};
