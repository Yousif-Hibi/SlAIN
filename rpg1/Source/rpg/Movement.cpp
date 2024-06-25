// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement.h"

void UMovement::UpdateCombatType(TEnumAsByte<ECombatType> combatType)
{
	IAnimInstance_CI::UpdateCombatType(combatType);
	CombatType = combatType;
}

void UMovement::UpdateCombatEnabled(bool bAttachToHand)
{
	IAnimInstance_CI::UpdateCombatEnabled(bAttachToHand);
	isAttach = bAttachToHand;
	
}


