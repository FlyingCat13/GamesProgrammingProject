// Copyright Epic Games, Inc. All Rights Reserved.


#include "AssignmentGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"

AAssignmentGameMode::AAssignmentGameMode()
{
    DefaultPawnClass = AMainPlayer::StaticClass();
}

// Set up all the widgets beforehand and start with menu state.
void AAssignmentGameMode::BeginPlay()
{
    Super::BeginPlay();

    SetUpMainMenu();
    SetUpInGameHUD();
    SetUpPauseScreen();
    SetUpWinScreen();
    SetUpLoseScreen();

    SwitchToMenuState();
}

// Update the time that the tooltip for interacted item. If duration expires, update the HUD.
void AAssignmentGameMode::Tick(float DeltaTime)
{
    if (InteractedItemTooltipDuration > 0.f)
    {
        InteractedItemTooltipDuration -= DeltaTime;
        if (InteractedItemTooltipDuration <= 0.f)
        {
            // Update the HUD
            UInGameHUD* InGameHUD = Cast<UInGameHUD>(Widgets[PlayState]);
            InGameHUD->InteractedItemTooltipTextBlock->SetText(FText::GetEmpty());
        }
    }
}

// Set up the main menu UI.
void AAssignmentGameMode::SetUpMainMenu()
{
    // Create the widget and add to the widget array.
    Widgets[MenuState] = CreateWidget<UUserWidget>(GetWorld(), UMainMenuUI::StaticClass());
    UMainMenuUI* MainMenu = Cast<UMainMenuUI>(Widgets[MenuState]);
    // Assign callbacks to start button (switch to play state) and quit button (quit the game).
    if (!MainMenu->StartButton->OnClicked.IsBound())
    {
        MainMenu->StartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::SwitchToPlayState);
    }
    if (!MainMenu->QuitButton->OnClicked.IsBound())
    {
        MainMenu->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
    }
}

// Set up the in game HUD.
void AAssignmentGameMode::SetUpInGameHUD()
{
    // Create the widget and add to the widget array.
    Widgets[PlayState] = CreateWidget<UUserWidget>(GetWorld(), UInGameHUD::StaticClass());
}

// Set up the pause screen
void AAssignmentGameMode::SetUpPauseScreen()
{
    // Create the widget and add to the widget array.
    Widgets[PauseState] = CreateWidget<UUserWidget>(GetWorld(), UPauseScreen::StaticClass());
}

// Set up the win screen
void AAssignmentGameMode::SetUpWinScreen()
{
    // Create the widget and add to the widget array.
    Widgets[WinState] = CreateWidget<UUserWidget>(GetWorld(), UWinScreen::StaticClass());
    // Assign callbacks to restart button (restart the game mode) and quit button (quit the game).
    UWinScreen* WinScreen = Cast<UWinScreen>(Widgets[WinState]);
    if (!WinScreen->RestartButton->OnClicked.IsBound())
    {
        WinScreen->RestartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::RestartGame);
    }
    if (!WinScreen->QuitButton->OnClicked.IsBound())
    {
        WinScreen->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
    }
}

// Set up the lose screen
void AAssignmentGameMode::SetUpLoseScreen()
{
    // Create the widget and add to the widget array.
    Widgets[LoseState] = CreateWidget<UUserWidget>(GetWorld(), ULoseScreen::StaticClass());
    // Assign callbacks to restart button (restart the game mode) and quit button (quit the game).
    ULoseScreen* LoseScreen = Cast<ULoseScreen>(Widgets[LoseState]);
    if (!LoseScreen->RestartButton->OnClicked.IsBound())
    {
        LoseScreen->RestartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::RestartGame);
    }
    if (!LoseScreen->QuitButton->OnClicked.IsBound())
    {
        LoseScreen->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
    }
}

// Change the on screen widget
void AAssignmentGameMode::ChangeOnScreenWidget(GameState OldState, GameState NewState)
{
    // Remove old state widget from viewport and add new state widget instead.
    Widgets[OldState]->RemoveFromViewport();
    if (Widgets[NewState])
    {
        Widgets[NewState]->AddToViewport(9999);
    }
}

void AAssignmentGameMode::OnPressPause()
{
    // When in play state, pressing pause button will switch to pause state.
    // When in pause state, pressing pause button will switch to play state.
    if (CurrentState == PlayState)
    {
        SwitchToPauseState();
    }
    else if (CurrentState == PauseState)
    {
        SwitchToPlayState(); 
    }
}

// Expose switching to win state publicly for other class to use.
void AAssignmentGameMode::OnWin()
{
    SwitchToWinState();
}

// Expose switching to lose state publicly for other class to use.
void AAssignmentGameMode::OnLose()
{
    UE_LOG(LogClass, Warning, TEXT("This a testing statement."));
    SwitchToLoseState();
}

// Quit game callback to be assigned to the quit button.
void AAssignmentGameMode::OnQuit()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
    }
}

// Update the look at item HUD tooltip with the input tooltip.
void AAssignmentGameMode::OnLookAtItemTooltipUpdate(FText Tooltip)
{
    UInGameHUD* InGameHUD = Cast<UInGameHUD>(Widgets[PlayState]);
    InGameHUD->LookAtItemTooltipTextBlock->SetText(Tooltip);
}

// Update the interacted item HUD tooltip with the input tooltip.
void AAssignmentGameMode::OnInteractedItemTooltipUpdate(FText Tooltip)
{
    UInGameHUD* InGameHUD = Cast<UInGameHUD>(Widgets[PlayState]);
    InGameHUD->InteractedItemTooltipTextBlock->SetText(Tooltip);
    InteractedItemTooltipDuration = 3.f;
}

// Update the inventory tooltip with the input array of inventory in text form and the currently equipped item.
void AAssignmentGameMode::OnUpdateInventoryStatus(TArray<FText> InventoryItemNames, int EquippedItem)
{
    UInGameHUD* InGameHUD = Cast<UInGameHUD>(Widgets[PlayState]);
    InGameHUD->UpdateInventoryInformation(InventoryItemNames, EquippedItem);
}

// Update the health bar by setting the health bar on the HUD to the input percentage.
void AAssignmentGameMode::OnUpdateHealthBar(float Percentage)
{
    UInGameHUD* InGameHUD = Cast<UInGameHUD>(Widgets[PlayState]);
    InGameHUD->HealthBar->SetPercent(Percentage);
}

// Switch to menu state 
void AAssignmentGameMode::SwitchToMenuState()
{
    UE_LOG(LogClass, Warning, TEXT("Menu"));
    // Switch widget
    ChangeOnScreenWidget(CurrentState, MenuState);
    // Switch state
    CurrentState = MenuState;
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    // Allow for mouse control in menu.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }
}

// Switch to play state
void AAssignmentGameMode::SwitchToPlayState()
{
    UE_LOG(LogClass, Warning, TEXT("Play"));
    // Switch widget
    ChangeOnScreenWidget(CurrentState, PlayState);
    // Switch state
    CurrentState = PlayState;
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    // Disable mouse cursor.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->bEnableClickEvents = false;
        PlayerController->bEnableMouseOverEvents = false;
    }
}

void AAssignmentGameMode::SwitchToPauseState()
{
    UE_LOG(LogClass, Warning, TEXT("Pause"));
    // Switch widget
    ChangeOnScreenWidget(CurrentState, PauseState);
    // Switch state
    CurrentState = PauseState;
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AAssignmentGameMode::SwitchToWinState()
{
    UE_LOG(LogClass, Warning, TEXT("Win"));
    // Switch widget
    ChangeOnScreenWidget(CurrentState, WinState);
    // Switch state
    CurrentState = WinState;
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    // Allow cursor control in win screen
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }
}

void AAssignmentGameMode::SwitchToLoseState()
{
    UE_LOG(LogClass, Warning, TEXT("Lose"));
    // Switch widget
    ChangeOnScreenWidget(CurrentState, LoseState);
    // Switch state
    CurrentState = LoseState;
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    // Allow cursor control in lose screen
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }
}