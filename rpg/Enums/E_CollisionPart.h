// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "E_CollisionPart.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UE_CollisionPart : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
UENUM(BlueprintType)
enum ECollisionPart
{
	MainWeapon,
	OffhandWeeapon,
	RightFoot
};
