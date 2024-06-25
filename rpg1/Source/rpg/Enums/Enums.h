// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Enums.generated.h"

UENUM(BlueprintType,Category="GameRules")
enum ERotationMode
{
	OrientToCamara,
	OrintToMovment
};

