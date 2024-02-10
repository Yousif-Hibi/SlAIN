// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EState.generated.h"

/**
 * 
 */
UCLASS()
class  UEState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType, Category = "GameRules")
enum Estat
{
	NoState,
	Health,
	Stamina,
	Armor
};

