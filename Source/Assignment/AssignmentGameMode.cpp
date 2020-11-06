// Copyright Epic Games, Inc. All Rights Reserved.


#include "AssignmentGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainPlayer.h"

AAssignmentGameMode::AAssignmentGameMode()
{
    DefaultPawnClass = AMainPlayer::StaticClass();

    // Determine the UI classes that will be used.
    WidgetClass[MenuState] = UMainMenuUI::StaticClass();
    WidgetClass[PlayState] = UInGameHUD::StaticClass();
    WidgetClass[PauseState] = UPauseScreen::StaticClass();
    WidgetClass[LoseState] = ULoseScreen::StaticClass();
    WidgetClass[WinState] = UWinScreen::StaticClass();
    WidgetClass[CutSceneState] = UBlankWidget::StaticClass();

    ConstructorHelpers::FObjectFinder<ULevelSequence> LevelSequenceAsset(TEXT("/Game/Cinematics/Master.Master"));
    if (LevelSequenceAsset.Succeeded())
    {
        LevelSequence = LevelSequenceAsset.Object;
    }
}

// Set up all the widgets beforehand and start with menu state.
void AAssignmentGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Only play cut scene when first played.
    bool CutScene = UGameplayStatics::ParseOption(OptionsString, "CutScene") == "True";
    bool Play = UGameplayStatics::ParseOption(OptionsString, "Play") == "True";

    if (Play)
    {
        SwitchToMenuState();
    }
    else if (CutScene)
    {
        SwitchToCutSceneState();

        ALevelSequenceActor* OutActor;
        if (LevelSequence)
        {
            ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
            if (LevelSequencePlayer)
            {
                LevelSequencePlayer->OnFinished.AddDynamic(this, &AAssignmentGameMode::FinishCutScene);
                LevelSequencePlayer->Play();
            }
        }
    }
    else
    {
        // Switch to sequence level
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/SequencerLevel"), false, "CutScene=True");
    }

    // Subscribe to the player die event to trigger OnLose() function when the player dies.
    AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        Player->OnPlayerDieEvent.AddDynamic(this, &AAssignmentGameMode::OnLose);
    }
}

// Update the time that the tooltip for interacted item. If duration expires, update the HUD.
void AAssignmentGameMode::Tick(float DeltaTime)
{
    // Update the ingame HUD if currently in play state.
    if (CurrentState == PlayState)
    {
        UInGameHUD* InGameHUD = Cast<UInGameHUD>(CurrentWidget);
        AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        UpdateTooltips(InGameHUD, DeltaTime);
        UpdateHealthBar(InGameHUD, Player);
        UpdateInventoryStatus(InGameHUD, Player);
    }
}

// Update the tooltips on screen.
void AAssignmentGameMode::UpdateTooltips(UInGameHUD* InGameHUD, float DeltaTime)
{
    if (InteractedItemTooltipDuration > 0.f)
    {
        InteractedItemTooltipDuration -= DeltaTime;
        if (InteractedItemTooltipDuration <= 0.f)
        {
            // Update the tooltip
            InteractedItemTooltip = FText::GetEmpty();
        }
    }

    InGameHUD->InteractedItemTooltipTextBlock->SetText(InteractedItemTooltip);
    InGameHUD->LookAtItemTooltipTextBlock->SetText(LookAtItemTooltip);
}

// Update the healthbar percentage.
void AAssignmentGameMode::UpdateHealthBar(UInGameHUD* InGameHUD, AMainPlayer* Player)
{
    InGameHUD->HealthBar->SetPercent(Player->GetHealthPercentage());
}

// Update the player's inventory status.
void AAssignmentGameMode::UpdateInventoryStatus(UInGameHUD* InGameHUD, AMainPlayer* Player)
{
    InGameHUD->UpdateInventoryInformation(Player->InventoryInText(), Player->GetSelectedInventorySlot());
}

// Change the on screen widget
void AAssignmentGameMode::ChangeOnScreenWidget(GameState NewState)
{   
    // Switch state
    CurrentState = NewState;

    // Remove old state widget from viewport and add new state widget instead.
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
    }
    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass[NewState]);
    CurrentWidget->AddToViewport(9999);

    // Attach callbacks to buttons on applicable widgets.
    SetupCurrentWidget();
}

// Initial binding for widget's buttons when it is created.
void AAssignmentGameMode::SetupCurrentWidget()
{
    switch (CurrentState)
    {
        case MenuState:
        {
            UMainMenuUI* MainMenu = Cast<UMainMenuUI>(CurrentWidget);
            // Assign callbacks to start button (switch to play state) and quit button (quit the game).
            if (!MainMenu->StartButton->OnClicked.IsBound())
            {
                MainMenu->StartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::SwitchToPlayState);
            }
            if (!MainMenu->QuitButton->OnClicked.IsBound())
            {
                MainMenu->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
            }
            
            break;
        }
        case WinState:
        {
            UWinScreen* WinScreen = Cast<UWinScreen>(CurrentWidget);
            // Assign callbacks to restart button (restart the game mode) and quit button (quit the game).
            if (!WinScreen->RestartButton->OnClicked.IsBound())
            {
                WinScreen->RestartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::RestartGame);
            }
            if (!WinScreen->QuitButton->OnClicked.IsBound())
            {
                WinScreen->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
            }

            break;
        }
        case LoseState:
        {
            ULoseScreen* LoseScreen = Cast<ULoseScreen>(CurrentWidget);
            // Assign callbacks to restart button (restart the game mode) and quit button (quit the game).
            if (!LoseScreen->RestartButton->OnClicked.IsBound())
            {
                LoseScreen->RestartButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::RestartGame);
            }
            if (!LoseScreen->QuitButton->OnClicked.IsBound())
            {
                LoseScreen->QuitButton->OnClicked.AddDynamic(this, &AAssignmentGameMode::OnQuit);
            }

            break;
        }
        default:
        {
            break;
        }
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
    LookAtItemTooltip = Tooltip;
}

// Update the interacted item HUD tooltip with the input tooltip.
void AAssignmentGameMode::OnInteractedItemTooltipUpdate(FText Tooltip)
{
    InteractedItemTooltip = Tooltip;
    InteractedItemTooltipDuration = 3.f;
}

// Switch to menu state 
void AAssignmentGameMode::SwitchToMenuState()
{
    UE_LOG(LogClass, Warning, TEXT("Menu"));
    // Switch widget
    ChangeOnScreenWidget(MenuState);
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
    ChangeOnScreenWidget(PlayState);
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
    ChangeOnScreenWidget(PauseState);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AAssignmentGameMode::SwitchToWinState()
{
    UE_LOG(LogClass, Warning, TEXT("Win"));
    // Switch widget
    ChangeOnScreenWidget(WinState);
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
    ChangeOnScreenWidget(LoseState);
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

void AAssignmentGameMode::SwitchToCutSceneState()
{
    UE_LOG(LogClass, Warning, TEXT("Cut Scene"));
    // Switch widget
    ChangeOnScreenWidget(CutSceneState);
    UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AAssignmentGameMode::FinishCutScene()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/GameLevel0"), false, "Play=True");
}