// Fill out your copyright notice in the Description page of Project Settings.


#include "settings.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericWindow.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h" 


void Usettings::NativeConstruct()
{

   
    if (GraphicsPlus)
    {
        GraphicsPlus->OnClicked.AddDynamic(this, &Usettings::OnGraphicsPlusClicked);
    }

    if (GraphicsLess)
    {
        GraphicsLess->OnClicked.AddDynamic(this, &Usettings::OnGraphicsLessClicked);
    }

    if (ResolutionPlus)
    {
        ResolutionPlus->OnClicked.AddDynamic(this, &Usettings::OnResolutionPlusClicked);
    }

    if (ResolutionLess)
    {
        ResolutionLess->OnClicked.AddDynamic(this, &Usettings::OnResolutionLessClicked);
    }

    if (VsyncOn)
    {
        VsyncOn->OnClicked.AddDynamic(this, &Usettings::OnVsyncOnClicked);
    }

    if (VsyncOff)
    {
        VsyncOff->OnClicked.AddDynamic(this, &Usettings::OnVsyncOffClicked);
    }

    if (Apply)
    {
        Apply->OnClicked.AddDynamic(this, &Usettings::OnApplyClicked);
    }

    if (Back)
    {
        Back->OnClicked.AddDynamic(this, &Usettings::OnBackClicked);
    }
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
   
    GraphicsIndex = UserSettings->GetVisualEffectQuality();
    Resolution = UserSettings->GetScreenResolution();
    ResolutionIndex = 0;  
    bVsync = UserSettings->IsVSyncEnabled();
}






void Usettings::OnGraphicsPlusClicked()
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    GraphicsIndex = FMath::Clamp(GraphicsIndex + 1, 0, 4);
    GEngine->GetGameUserSettings()->SetVisualEffectQuality(GraphicsIndex);
}

void Usettings::OnGraphicsLessClicked()
{
    GraphicsIndex = FMath::Clamp(GraphicsIndex - 1, 0, 3);
    GEngine->GetGameUserSettings()->SetVisualEffectQuality(GraphicsIndex);
}

void Usettings::OnResolutionPlusClicked()
{
    TArray<FIntPoint> Resolutions = {
        FIntPoint(1280, 720),
        FIntPoint(1920, 1080),
        FIntPoint(2560, 1440),
        FIntPoint(3840, 2160)
    };
    ResolutionIndex = (ResolutionIndex + 1) % Resolutions.Num();
    Resolution = Resolutions[ResolutionIndex];
    GEngine->GetGameUserSettings()->SetScreenResolution(Resolution);
}

void Usettings::OnResolutionLessClicked()
{
    TArray<FIntPoint> Resolutions = {
        FIntPoint(1280, 720),
        FIntPoint(1920, 1080),
        FIntPoint(2560, 1440),
        FIntPoint(3840, 2160)
    };
    ResolutionIndex = (ResolutionIndex + Resolutions.Num() - 1) % Resolutions.Num();
    Resolution = Resolutions[ResolutionIndex];
    GEngine->GetGameUserSettings()->SetScreenResolution(Resolution);
}

void Usettings::OnVsyncOnClicked()
{
    bVsync = true;
    GEngine->GetGameUserSettings()->SetVSyncEnabled(bVsync);
}

void Usettings::OnVsyncOffClicked()
{
    bVsync = false;
    GEngine->GetGameUserSettings()->SetVSyncEnabled(bVsync);
}

void Usettings::OnApplyClicked()
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
   
    UserSettings->SetScreenResolution(Resolution);
    UserSettings->SetVSyncEnabled(bVsync);
   
    UserSettings->SetVisualEffectQuality(1);

    if (GEngine)
    {
       
        if (UserSettings)
        {
            UserSettings->SetViewDistanceQuality(0);   // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetAntiAliasingQuality(0);   // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetShadowQuality(0);         // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetPostProcessingQuality(0); // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetTextureQuality(0);        // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetVisualEffectQuality(0);   // 0 to 3: Low, Medium, High, Epic
            UserSettings->SetFoliageQuality(0);        // 0 to 3: Low, Medium, High, Epic

            // Apply the settings and save them
            UserSettings->ApplySettings(true);
        }
    }
    UserSettings->ApplySettings(true);
}

void Usettings::OnBackClicked()
{
    this->RemoveFromParent();
    if (WidgetClass)
    {

        UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

        if (WidgetInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("Settings widget created: %s"), *WidgetInstance->GetClass()->GetName());
            // Add widget to viewport
            WidgetInstance->AddToViewport();
        }
    }
}