// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "E_CharacterAction.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UE_CharacterAction : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType)
enum EChartacterAction
{
	SprintAttack,
	LightAttack,
	HeavyAttack,
	ChargedAttack,
	FallingAttack,
	Dodge,
	EnterCombat,
	ExitCombat,
	NoAction
};
