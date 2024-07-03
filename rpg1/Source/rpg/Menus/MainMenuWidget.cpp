// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    }
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
    }
}

void UMainMenuWidget::OnPlayClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    // Set input mode to game only
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }

    UGameplayStatics::OpenLevel(GetWorld(), "IronClanMap");
    
}

void UMainMenuWidget::OnSettingsClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("settings worked "));
    // Open settings menu or widget
}

void UMainMenuWidget::OnQuitClicked()
{
    
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
