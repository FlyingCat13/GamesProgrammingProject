// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InGameHUD.h"
#include "LoseScreen.h"
#include "MainMenuUI.h"
#include "PauseScreen.h"
#include "WinScreen.h"
#include "AssignmentGameMode.generated.h"

// Predeclare AMainPlayer
class AMainPlayer;

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AAssignmentGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Game states enum for different states
	enum GameState : size_t {
		MenuState = 0,
		PlayState = 1,
		PauseState = 2,
		WinState = 3,
		LoseState = 4,
	};
	// Current game state
	GameState CurrentState;

	// The current widget on screen.
	UUserWidget* CurrentWidget;
	// The Widget classes that would be used.
	TSubclassOf<UUserWidget> WidgetClass[5];

	// Tooltip for the interactable item that the player is looking at.
	FText LookAtItemTooltip;
	// Tooltip for the interactable item that the player interacted with.
	FText InteractedItemTooltip;
	// Duration for the interact tooltip to stay on screen.
	float InteractedItemTooltipDuration = 0.f;

	// Change on screen overlay based on the state that is being changed to.
	void ChangeOnScreenWidget(GameState NewState);
	// UI Widget setup.
	void SetupCurrentWidget();

	// HUD update functions
	void UpdateTooltips(UInGameHUD* InGameHUD, float DeltaTime);
	void UpdateHealthBar(UInGameHUD* InGameHUD, AMainPlayer* Player);
	void UpdateInventoryStatus(UInGameHUD* InGameHUD, AMainPlayer* Player);

	// Functions to switch states.
	void SwitchToMenuState();
	// UFUNCTION to allow this to be used with AddDynamic.
	UFUNCTION()
		void SwitchToPlayState();
	void SwitchToPauseState();
	void SwitchToWinState();
	void SwitchToLoseState();

	// Callback for quitting the game.
	UFUNCTION()
		void OnQuit();

public:
	AAssignmentGameMode();

	// Functions for player class to communicate to game mode.
	void OnPressPause();
	void OnWin();
	// UFUNCTION to allow this to be used with AddDynamic.
	UFUNCTION()
		void OnLose();
	void OnLookAtItemTooltipUpdate(FText Tooltip);
	void OnInteractedItemTooltipUpdate(FText Tooltip);
};
