// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "rpg/Enums/E_DamageType.h"
#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "C_AttackDamageType.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UC_AttackDamageType : public UDamageType
{
	GENERATED_BODY()
	
public :
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDamageType> DamageType = EDamageType::MeleDamage;







};
