// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "E_CombatType.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UE_CombatType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType, Category = "GameRules")
enum ECombatType 
{
	None,
	Lightsword,
	GreatSword,
	Axe,
	DualSword,
	MagicHand
};

