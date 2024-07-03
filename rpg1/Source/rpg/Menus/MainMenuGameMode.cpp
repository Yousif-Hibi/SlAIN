// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);

        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
            UGameplayStatics::SetGamePaused(GetWorld(), true);
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PlayerController)
            {
                PlayerController->bShowMouseCursor = true;
                PlayerController->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}
