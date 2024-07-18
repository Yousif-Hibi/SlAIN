// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* PlayButton;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> WidgetClass;

    UPROPERTY(meta = (BindWidget))
    class UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnQuitClicked();
};
