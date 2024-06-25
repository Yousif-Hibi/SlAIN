// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "E_ChartacterState.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UE_ChartacterState : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType, Category = "GameRules")
enum EChartacterState
{
	Nothing,
	Attacking,
	Dodging,
	GeneralActionState,
	Dead,
	Disabled
};
