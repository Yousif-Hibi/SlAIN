// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interfaces/AnimInstance_CI.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Movement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API UMovement : public UAnimInstance, public  IAnimInstance_CI
{
	GENERATED_BODY()
public:	

	virtual void UpdateCombatType(TEnumAsByte< ECombatType> combatType) override;

	virtual void UpdateCombatEnabled(bool bAttachToHand) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte< ECombatType> CombatType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttach= false;
};
