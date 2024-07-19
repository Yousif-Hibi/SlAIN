// Fill out your copyright notice in the Description page of Project Settings.


#include "WB_PauseUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "rpg/SoulsLikeCharacter.h"

void UWB_PauseUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UWB_PauseUserWidget::ResumeGame);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UWB_PauseUserWidget::QuitGame);
    }
}

void UWB_PauseUserWidget::ResumeGame()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    RemoveFromParent();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}

void UWB_PauseUserWidget::QuitGame()
{

    if (ASoulsLikeCharacter* PlayerCharacter = Cast<ASoulsLikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
        PlayerCharacter->SaveGame();
    }
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);

}