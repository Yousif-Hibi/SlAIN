// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionUserWidget.h"
#include "rpg/SoulsLikeCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h" 
#include "rpg/Component/C_StatsComponent.h"

void UOptionUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PlayerCharacter = Cast<ASoulsLikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    Points = PlayerCharacter->CharacterPoints;
    if (HealthLess)
    {
        HealthLess->OnClicked.AddDynamic(this, &UOptionUserWidget::OnHealthLessClicked);
    }
    if (HealthPluse)
    {
        HealthPluse->OnClicked.AddDynamic(this, &UOptionUserWidget::OnHealthPluseClicked);
    }
    if (StaminaLess)
    {
        StaminaLess->OnClicked.AddDynamic(this, &UOptionUserWidget::OnStaminaLessClicked);
    }
    if (StaminaPluse)
    {
        StaminaPluse->OnClicked.AddDynamic(this, &UOptionUserWidget::OnStaminaPluseClicked);
    }
    if (DamageLess)
    {
        DamageLess->OnClicked.AddDynamic(this, &UOptionUserWidget::OnDamageLessClicked);
    }
    if (DamagePluse)
    {
        DamagePluse->OnClicked.AddDynamic(this, &UOptionUserWidget::OnDamagePluseClicked);
    }
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UOptionUserWidget::OnBackButtonClicked);
    }
}

void UOptionUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    Points = PlayerCharacter->CharacterPoints;
    HealthPoints = PlayerCharacter->Healthpoints;
    StaminaPoints = PlayerCharacter->Staminapoints;
    DamagePoints = PlayerCharacter->Damagepoints;

}

void UOptionUserWidget::OnButtonClick()
{
       
}

void UOptionUserWidget::OnHealthLessClicked()
{
    if (PlayerCharacter->Healthpoints > 0) {
        if (PlayerCharacter->CharacterPoints >= 5) {

            PlayerCharacter->CharacterPoints = PlayerCharacter->CharacterPoints + 5;
            PlayerCharacter->Healthpoints = PlayerCharacter->Healthpoints + 1;
           
        }

    }
}

void UOptionUserWidget::OnHealthPluseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("HealthPluse button clicked"));
    if (PlayerCharacter->Healthpoints >= 0) {
        if (PlayerCharacter->CharacterPoints >= 5) {

            Points = Points - 5;
            PlayerCharacter->Healthpoints = PlayerCharacter->Healthpoints + 1;
           
        }

    }
}

void UOptionUserWidget::OnStaminaLessClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("StaminaLess button clicked"));
    if (PlayerCharacter->Staminapoints > 0) {
        if (PlayerCharacter->CharacterPoints >= 5) {

            Points = Points + 5;
            PlayerCharacter->Staminapoints = PlayerCharacter->Staminapoints - 1;

        }

    }
}

void UOptionUserWidget::OnStaminaPluseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("StaminaPluse button clicked"));
    if (PlayerCharacter->Staminapoints >= 0) {
        if (PlayerCharacter->CharacterPoints >= 5) {

            Points = Points - 5;
            PlayerCharacter->Staminapoints = PlayerCharacter->Staminapoints + 1;

        }

    }
}

void UOptionUserWidget::OnDamageLessClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("DamageLess button clicked"));
    if (PlayerCharacter->Damagepoints > 0) {
        if (PlayerCharacter->Damagepoints >= 5) {

            Points = Points + 5;
            PlayerCharacter->Damagepoints = PlayerCharacter->Damagepoints - 1;

        }

    }
}

void UOptionUserWidget::OnDamagePluseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("DamageMore button clicked"));

    if (PlayerCharacter->Damagepoints >= 0) {
        if (PlayerCharacter->Damagepoints >= 5) {

            Points = Points - 5;
            PlayerCharacter->Damagepoints = PlayerCharacter->Damagepoints + 1;

        }

    }
}

void UOptionUserWidget::OnBackButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Back button clicked"));
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    RemoveFromParent();
    this->RemoveFromViewport();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
    PlayerCharacter->LoadSaveGame();

}
