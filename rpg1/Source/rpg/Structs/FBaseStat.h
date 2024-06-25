// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FBaseStat.generated.h" 
/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_API FBaseStat
{
public:


	GENERATED_BODY()



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;



	FBaseStat();
	~FBaseStat();
};
