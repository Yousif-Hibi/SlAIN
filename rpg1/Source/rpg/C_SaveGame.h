// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "C_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UC_SaveGame : public USaveGame
{
	GENERATED_BODY()
	






public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator PlayerRotation;
};
