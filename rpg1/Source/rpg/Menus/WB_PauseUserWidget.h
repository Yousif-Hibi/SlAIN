// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WB_PauseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UWB_PauseUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable)
    void ResumeGame();

    UFUNCTION(BlueprintCallable)
    void QuitGame();

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;
};
