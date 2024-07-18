// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindow.h"
#include "GameFramework/GameUserSettings.h"

#include "Blueprint/UserWidget.h"

#include "settings.generated.h"



/**
 * 
 */
UCLASS()
class RPG_API Usettings : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;



    

   

    UPROPERTY(meta = (BindWidget))
    UButton* GraphicsPlus;

    UPROPERTY(meta = (BindWidget))
    UButton* GraphicsLess;

    UPROPERTY(meta = (BindWidget))
    UButton* ResolutionPlus;

    UPROPERTY(meta = (BindWidget))
    UButton* ResolutionLess;

    UPROPERTY(meta = (BindWidget))
    UButton* VsyncOn;

    UPROPERTY(meta = (BindWidget))
    UButton* VsyncOff;

    UPROPERTY(meta = (BindWidget))
    UButton* Apply;

    UPROPERTY(meta = (BindWidget))
    UButton* Back;
    
    UPROPERTY()
    int32 ResolutionIndex;
    UPROPERTY()
    FIntPoint  Resolution;
    UPROPERTY()
    int32  GraphicsIndex;
    UPROPERTY()
    bool bVsync;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> WidgetClass;

    UFUNCTION()
    void OnGraphicsPlusClicked();

    UFUNCTION()
    void OnGraphicsLessClicked();

    UFUNCTION()
    void OnResolutionPlusClicked();

    UFUNCTION()
    void OnResolutionLessClicked();

    UFUNCTION()
    void OnVsyncOnClicked();

    UFUNCTION()
    void OnVsyncOffClicked();

    UFUNCTION()
    void OnApplyClicked();

    UFUNCTION()
    void OnBackClicked();
  
};
