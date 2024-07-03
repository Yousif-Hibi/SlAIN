// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMainMenuGameMode : public AGameModeBase
{
   GENERATED_BODY()

public:
  
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;
    UPROPERTY(EditAnywhere)
    class UUserWidget* MainMenuWidget;
};
