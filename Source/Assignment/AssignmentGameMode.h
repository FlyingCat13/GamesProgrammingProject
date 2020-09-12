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

	// Different widgets for different UI elements based on game state.
	UUserWidget* Widgets[5];

	// Tooltip for the interactable item that the player is looking at.
	FText LookAtItemTooltip;
	// Tooltip for the interactable item that the player interacted with.
	FText InteractedItemTooltip;
	// Duration for the interact tooltip to stay on screen.
	float InteractedItemTooltipDuration = 0.f;

	// Change on screen overlay based on the state that is being changed to.
	void ChangeOnScreenWidget(GameState OldState, GameState NewState);

	// Functions for setting up UI elements
	void SetUpMainMenu();
	void SetUpInGameHUD();
	void SetUpPauseScreen();
	void SetUpWinScreen();
	void SetUpLoseScreen();

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
	void OnLose();
	void OnLookAtItemTooltipUpdate(FText Tooltip);
	void OnInteractedItemTooltipUpdate(FText Tooltip);
	void OnUpdateInventoryStatus(TArray<FText> InventoryItemNames, int EquippedItem);
	void OnUpdateHealthBar(float Percentage);
};
